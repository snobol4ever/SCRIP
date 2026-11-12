#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_41_sh.h"
int core_icn_error(int code, DESCR_t val);
extern void *rt_ws_alloc_descr(size_t);
extern const char *icon_real_str(double r, char *buf, int bufsz);
extern int rt_proc_is_registered(const char *name);
extern unsigned rt_dtax_gen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static void pas_trace_print_value(FILE *f, DESCR_t v) {
    if (IS_INT_fn(v)) { fprintf(f, "%lld", (long long)v.i); return; }
    if (IS_REAL_fn(v)) { fprintf(f, "%g", v.r); return; }
    { const char *s = VARVAL_fn(v); fputs(s ? s : "", f); }
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_loadfunc_cstr_args(DESCR_t *args, int nargs) {
    for (int i = 0; i < nargs && i < 2; i++) {
        if (args[i].v != DT_S || !args[i].s) continue;
        uint32_t n = args[i].slen;
        if (n && args[i].s[n - 1] == 0) continue;
        char *b = (char *)ct_alloc((size_t)n + 1);
        if (!b) continue;
        memcpy(b, args[i].s, (size_t)n);
        b[n] = 0;
        args[i].s = b;
        args[i].slen = n + 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_nxttab(int col, const int *stops, int nstops, int gap) {
    for (int k = 0; k < nstops; k++) if (stops[k] > col) return stops[k];
    int base = stops[nstops - 1];
    int beyond = col - base;
    return base + ((beyond / gap) + 1) * gap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_numeric_tail_is_blank(const char *p) { while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v') p++; return *p == '\0'; }
int icn_numeric_is_c_hex(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v') p++;
    if (*p == '+' || *p == '-') p++;
    return p[0] == '0' && (p[1] == 'x' || p[1] == 'X');
}
static int icn_cvt_chars_ok(DESCR_t d) { return d.v == DT_S || d.v == DT_I || d.v == DT_R || d.v == DT_C || d.v == DT_BIG; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_cvt_int_ok(DESCR_t d) {
    if (IS_INT_fn(d) || IS_REAL_fn(d)) return 1;
    if (d.v == DT_S && d.s) {
        const char *p = rt_cstr_d(d);
        while (*p == ' ') p++;
        if (*p == '+' || *p == '-') p++;
        int dg = 0;
        while (*p >= '0' && *p <= '9') { p++; dg = 1; }
        while (*p == ' ') p++;
        return dg && !*p;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_argtype_raise(int code, DESCR_t val, DESCR_t *out) { core_icn_error(code, val); *out = FAILDESCR; return 1; }
static int icn_alnum_c(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int icn_cvt_num_ok(DESCR_t d) {
    if (IS_INT_fn(d) || IS_REAL_fn(d) || d.v == DT_BIG) return 1;
    if (d.v != DT_S || !d.s || icn_numeric_is_c_hex(rt_cstr_d(d))) return 0;
    const char *p = rt_cstr_d(d);
    while (*p == ' ') p++;
    if (*p == '+' || *p == '-') p++;
    if (!((*p >= '0' && *p <= '9') || (*p == '.' && p[1] >= '0' && p[1] <= '9'))) return 0;
    const char *q = p;
    while (*q >= '0' && *q <= '9') q++;
    if (q > p && (*q == 'r' || *q == 'R')) { q++; while (icn_alnum_c(*q)) q++; while (*q == ' ') q++; return !*q; }
    char *e;
    double x = strtod(p, &e);
    while (*e == ' ') e++;
    return !*e && !(isinf(x) && strpbrk(p, ".eE"));
}
int icn_builtin_is_leaf_row(const char *name) { int r = name ? bt_row_of(BT_VOC_ICN, name, (unsigned) strlen(name)) : -1; return r >= 0 && g_builtins[r].conv == BT_CONV_LEAF; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_generator(const char *name) {
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any") || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key") || !strcmp(name, "seq") ||
        !strcmp(name, "push") || !strcmp(name, "put");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int builtin_is_generator(const char *name) { return rt_builtin_is_generator(name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_generator(const char *name) {
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any") || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key") || !strcmp(name, "seq");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icon_radix_int(const char *s, long long *out) {
    const char *p = s;
    while (*p == ' ' || *p == '\t') p++;
    int neg = 0;
    if (*p == '+') p++;
    else if (*p == '-') { neg = 1; p++; }
    int base = 0;
    const char *bstart = p;
    while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
    if (!(p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36)) return 0;
    p++;
    long long v = 0;
    while (*p) {
        int d = -1;
        if (*p >= '0' && *p <= '9') d = *p - '0';
        else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
        if (d < 0 || d >= base) break;
        if (v > (9223372036854775807LL - d) / base) return -1;
        v = v * base + d;
        p++;
    }
    while (*p == ' ' || *p == '\t') p++;
    if (*p != '\0') return 0;
    *out = neg ? -v : v;
    return 1;
}
DESCR_t icon_radix_big(const char *s) {
    extern DESCR_t rt_big_from_str_base(const char *, int);
    extern DESCR_t rt_big_neg(DESCR_t);
    const char *p = s;
    while (*p == ' ' || *p == '\t') p++;
    int neg = 0;
    if (*p == '+') p++;
    else if (*p == '-') { neg = 1; p++; }
    int base = 0;
    while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
    p++;
    { DESCR_t r = rt_big_from_str_base(p, base); if (IS_FAIL_fn(r) || !neg) return r; return rt_big_neg(r); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_data_type_name(DESCR_t v) { return (IS_DATA_INST_fn(v) && v.u && ((DATINST_t *)v.u)->type && ((DATINST_t *)v.u)->type->name) ? ((DATINST_t *)v.u)->type->name : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_data_is_record(DESCR_t v) { const char *n = rt_data_type_name(v); return n[0] && strcmp(n, "list") != 0; }
int relop_radix_coerce(DESCR_t v, DESCR_t *out) {
    if (!IS_STR_fn(v) || IS_CSET_fn(v) || !v.s || v.slen == 0) return 0;
    char nb[128];
    const char *s = v.s;
    if (v.slen != 0xFFFFFFFFu) { char *cp = (v.slen < sizeof nb) ? nb : (char *)rt_wsb_alloc((size_t)v.slen + 1); memcpy(cp, v.s, v.slen); cp[v.slen] = '\0'; s = cp; }
    long long rv;
    int rr = icon_radix_int(s, &rv);
    if (rr > 0) { *out = INTVAL(rv); return 1; }
    if (rr < 0) { DESCR_t b = icon_radix_big(s); if (IS_FAIL_fn(b)) return 0; *out = b; return 1; }
    return 0;
}
void dtx5(dtax_ent_t *e, unsigned char l, const char *nm, void *h, short x) {
    if (!e || !l) return;
    e->gen = rt_dtax_gen;
    e->len = l;
    e->kind = 5;
    e->nf = x;
    memcpy(e->dxn, nm, l);
    e->ctor = h;
    e->syn = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bn_datatype_upper(const char *t) {
    switch (t[0]) {
        case 's':
        return !strcmp(t, "string") ? "STRING" : !strcmp(t, "set") ? "SET" : (const char *)0;
        case 'i':
        return !strcmp(t, "integer") ? "INTEGER" : (const char *)0;
        case 'r':
        return !strcmp(t, "real") ? "REAL" : !strcmp(t, "record") ? "RECORD" : (const char *)0;
        case 't':
        return !strcmp(t, "table") ? "TABLE" : (const char *)0;
        case 'a':
        return !strcmp(t, "array") ? "ARRAY" : (const char *)0;
        case 'l':
        return !strcmp(t, "list") ? "LIST" : (const char *)0;
        case 'c':
        return !strcmp(t, "cset") ? "CSET" : !strcmp(t, "co-expression") ? "CO-EXPRESSION" : (const char *)0;
        case 'f':
        return !strcmp(t, "file") ? "FILE" : !strcmp(t, "function") ? "FUNCTION" : (const char *)0;
        case 'p':
        return !strcmp(t, "procedure") ? "PROCEDURE" : (const char *)0;
        case 'n':
        return !strcmp(t, "name") ? "NAME" : !strcmp(t, "null") ? "NULL" : (const char *)0;
        case 'E':
        return !strcmp(t, "EXPRESSION") ? "EXPRESSION" : (const char *)0;
        case 'P':
        return !strcmp(t, "PATTERN") ? "PATTERN" : (const char *)0;
        case 'C':
        return !strcmp(t, "CODE") ? "CODE" : (const char *)0;
        default:
        return (const char *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_type_datatype(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t av = args[0];
    const char *t;
    int declared = 0;
    (void)nargs;
    if (av.v == DT_EXTL) { extern DESCR_t rt_extl_name(DESCR_t); *out = rt_extl_name(av); return 1; }
    if (av.v == DT_CO) t="co-expression";
    else if (av.v == DT_BIG) t="integer";
    else if (IS_INT_fn(av)) t="integer";
    else if (IS_REAL_fn(av)) t="real";
    else if (av.v==DT_T) t=(av.tbl && av.tbl->is_set) ? "set" : "table";
    else if (av.v==DT_A) t=(!strcmp(fn,"DATATYPE")) ? "array" : "list";
    else if (av.v==DT_DATA) {
        if (rt_lv_is_list(av, (DESCR_t **)0, (int *)0)) t = "list";
        else { DATINST_t *di = (DATINST_t *)av.u; declared = (di && di->type && di->type->name) ? 1 : 0; t = declared ? di->type->name : "record"; }
    } else if (IS_CSET_fn(av)) t="cset";
    else if (IS_FH_fn(av)) t="file";
    else if (av.v==DT_E && av.slen == RT_CONVE_CHAIN_MARK) t="EXPRESSION";
    else if (av.v==DT_E) {
        t = "procedure";
        if (!strcmp(fn,"DATATYPE")) {
            t = "function";
            if (IS_PROCVAL_fn(av) && av.s) {
                for (int _ti=0;_ti<g_stage2.proc_count;_ti++) if (g_stage2.proc_table[_ti].name && !strcmp(g_stage2.proc_table[_ti].name,av.s)){ t="procedure"; break; }
                if (!strcmp(t,"function")) { extern int rt_proc_is_registered(const char *); if (rt_proc_is_registered(av.s)) t="procedure"; }
            } else t="procedure";
        }
    } else if (av.v==DT_C) t="CODE";
    else if (av.v==DT_X) t="EXPRESSION";
    else if (av.v==DT_N) t="name";
    else if (av.v==DT_P) t=(!strcmp(fn,"DATATYPE")) ? "PATTERN" : "pattern";
    else if (av.v==DT_SNUL) t=(!strcmp(fn,"DATATYPE")) ? "string" : "null";
    else t="string";
    if (!strcmp(fn,"DATATYPE") && declared) { *out = STRVAL(rt_heap_strdup_c(t)); return 1; }
    if (!strcmp(fn,"DATATYPE")) { const char *uc = bn_datatype_upper(t); if (uc) { *out = STRVAL(uc); return 1; } }
    if (!strcmp(fn,"DATATYPE")) {
        char *ub = (char *)rt_heap_alloc_c(strlen(t) + 1);
        int ui=0;
        for (; t[ui]; ui++) ub[ui]=(char)((t[ui]>='a'&&t[ui]<='z')?t[ui]-32:t[ui]);
        ub[ui]=0;
        *out = STRVAL(ub);
        return 1;
    }
    *out = STRVAL(t);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_sno_name(DESCR_t *args, int nargs, DESCR_t *out) {
    const char *sv;
    (void)nargs;
    if (IS_VARREF_fn(args[0])) { *out = args[0]; return 1; }
    if (args[0].v == DT_S && args[0].s && args[0].slen > 0 && args[0].slen != 0xFFFFFFFFu) {
        extern DESCR_t *NV_CELL_FAST_n(const char *, size_t);
        extern int comm_var_active(void);
#if RT_DIAG
        DESCR_t *fc = comm_var_active() ? (DESCR_t *)0 : NV_CELL_FAST_n(args[0].s, (size_t)args[0].slen);
#else
        DESCR_t *fc = NV_CELL_FAST_n(args[0].s, (size_t)args[0].slen);
#endif
        if (fc) { *out = (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)fc }; return 1; }
    }
    if (args[0].v == DT_S && args[0].s && args[0].slen > 0 && args[0].slen != 0xFFFFFFFFu && NV_name_needs_key(args[0].s, (size_t)args[0].slen)) {
        DESCR_t *cell = NV_PTR_n(args[0].s, (size_t)args[0].slen);
        *out = cell ? (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)cell } : FAILDESCR;
        return 1;
    }
    if (args[0].v == DT_S && args[0].s && args[0].slen > 0 && args[0].slen != 0xFFFFFFFFu) {
        extern const char *NV_intern_name_n(const char *, size_t);
        const char *in = NV_intern_name_n(args[0].s, (size_t)args[0].slen);
        if (in) {
            extern int comm_var_active(void);
            extern DESCR_t *NV_CELL_IF_FASTSET_fn(const char *);
            const int special = (in[0] == 'I' && !strcmp(in, "INPUT")) || (in[0] == 'O' && !strcmp(in, "OUTPUT")) || (in[0] == 'T' && !strcmp(in, "TERMINAL")) ||
                (is_protected_pat_lead(in[0]) && is_protected_pat_name(in));
#if RT_DIAG
            DESCR_t *cell = (!special && !comm_var_active()) ? NV_CELL_IF_FASTSET_fn(in) : (DESCR_t *)0;
#else
            DESCR_t *cell = (!special) ? NV_CELL_IF_FASTSET_fn(in) : (DESCR_t *)0;
#endif
            *out = cell ? (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)cell } : (DESCR_t){ .v = DT_N, .slen = 0, .s = in };
            return 1;
        }
    }
    sv = rt_sno_indirect_name(args[0]);
    if (!sv || !*sv) { *out = FAILDESCR; return 1; }
    {
        extern const char *NV_intern_name_fn(const char *);
        const char *in = NV_intern_name_fn(sv);
        DESCR_t d;
        memset(&d, 0, sizeof d);
        d.v = DT_N;
        d.slen = 0;
        d.s = in ? in : rt_heap_strdup_c(sv);
        *out = d;
        return 1;
    }
}
#if RT_DIAG
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bidprof_dump(void) { for (int i = 0; i < 1024; i++) if (g_runtime.bn.bidprof[i]) fprintf(stderr, "BIDPROF %d %ld\n", i, g_runtime.bn.bidprof[i]); }
void bidprof_init(void) { const char *e = getenv("SCRIP_BID_PROF"); g_runtime.bn.bidprof_on = (e && e[0]=='1') ? 1 : 0; if (g_runtime.bn.bidprof_on) atexit(bidprof_dump); }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sort_key_cstr(DESCR_t v, char *buf, int bufsz) {
    if (v.v == DT_S) return v.s ? v.s : "";
    if (v.v == DT_SNUL) return "";
    if (v.v == DT_I) { snprintf(buf, (size_t)bufsz, "%lld", (long long)v.i); return buf; }
    if (IS_FH_fn(v)) {
        int idx = (int)v.i;
        if (idx == 0) return "&input";
        if (idx == 1) return "&output";
        if (idx == 2) return "&errout";
        if (idx >= 0 && idx < FH_N && g_fh[idx].name) { snprintf(buf, (size_t)bufsz, "file(%s)", g_fh[idx].name); return buf; }
        return "file(?)";
    }
    { const char *s = VARVAL_fn(v); return s ? s : ""; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_type_rank(DESCR_t v, int num) {
    if (v.v == DT_SNUL) return 0;
    if (num && (v.v == DT_I || v.v == DT_BIG || v.v == DT_R)) return 1;
    if (v.v == DT_I || v.v == DT_BIG) return 1;
    if (v.v == DT_R) return 2;
    if (IS_CSET_fn(v)) return 4;
    if (v.v == DT_S || v.v == DT_N) return 3;
    if (IS_FH_fn(v)) return 5;
    if (v.v == DT_CO) return 6;
    if (v.v == DT_E) return 7;
    if (v.v == DT_A) return 8;
    if (v.v == DT_T) return (v.tbl && v.tbl->is_set) ? 9 : 10;
    if (v.v == DT_DATA) return rt_lv_is_list(v, (DESCR_t **)0, (int *)0) ? 8 : 11;
    if (v.v == DT_EXTL) return 12;
    return 3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long sort_struct_serial(DESCR_t v) {
    if (v.v == DT_CO && v.p) { extern long scrip_coexpr_serial_of(void *); return scrip_coexpr_serial_of(v.p); }
    if (v.v == DT_A && v.arr) return v.arr->id;
    if (v.v == DT_T && v.tbl) return v.tbl->id;
    if (IS_DATA_INST_fn(v) && v.u) return ((DATINST_t *)v.u)->id;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sort_struct_type_name(DESCR_t v) { extern const char *rt_data_type_name(DESCR_t); return rt_data_type_name(v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_is_record(DESCR_t v) { extern int rt_data_is_record(DESCR_t); return rt_data_is_record(v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_chars_len(DESCR_t d) { int n = (int)descr_slen(d); if (n == 0 && d.s && *d.s) n = (int)strlen(d.s); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_chars_cmp(DESCR_t a, DESCR_t b) {
    const char *sa = a.s ? a.s : "", *sb = b.s ? b.s : "";
    int la = sort_chars_len(a), lb = sort_chars_len(b), n = la < lb ? la : lb, c = n > 0 ? memcmp(sa, sb, (size_t)n) : 0;
    if (c) return c < 0 ? -1 : 1;
    return la < lb ? -1 : (la > lb ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_long_cmp(long x, long y) { return x < y ? -1 : (x > y ? 1 : 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sort_proc_name(DESCR_t v) {
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].entry_pc == (int)v.i && g_stage2.proc_table[i].name) return g_stage2.proc_table[i].name;
    return v.s ? v.s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sortf_field_i(DESCR_t *args, int nargs) { return (nargs < 2 || args[1].v == DT_SNUL || IS_FAIL_fn(args[1])) ? 1 : (int)to_int(args[1]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_key_str_cmp(DESCR_t a, DESCR_t b) {
    char ba[64], bb[64];
    int c = strcmp(sort_key_cstr(a, ba, (int)sizeof ba), sort_key_cstr(b, bb, (int)sizeof bb));
    return c < 0 ? -1 : (c > 0 ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_descr_cmp(DESCR_t a, DESCR_t b, int num) {
    int ra = sort_type_rank(a, num), rb = sort_type_rank(b, num);
    if (ra != rb) return ra < rb ? -1 : 1;
    if (ra == 0) return 0;
    if (ra == 1) {
        if (a.v == DT_R || b.v == DT_R) { double x = to_real(a), y = to_real(b); return x > y ? 1 : (x < y ? -1 : 0); }
        if (a.v == DT_I && b.v == DT_I) return (a.i > b.i) ? 1 : (a.i < b.i) ? -1 : 0;
        { extern int rt_big_cmp(DESCR_t, DESCR_t); int c = rt_big_cmp(a, b); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    }
    if (ra == 2) return (a.r > b.r) ? 1 : (a.r < b.r) ? -1 : 0;
    if (ra == 3 || ra == 4) return sort_chars_cmp(a, b);
    if (ra == 5) return sort_key_str_cmp(a, b);
    if (ra == 7) { int c = strcmp(sort_proc_name(a), sort_proc_name(b)); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    if (ra == 11) { int c = strcmp(sort_struct_type_name(a), sort_struct_type_name(b)); if (c) return c < 0 ? -1 : 1; }
    if (ra == 12) { extern int rt_extl_cmp(DESCR_t, DESCR_t); return rt_extl_cmp(a, b); }
    return sort_long_cmp(sort_struct_serial(a), sort_struct_serial(b));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_field_slot(int field_i, int n) { int idx = field_i > 0 ? field_i - 1 : n + field_i; return (field_i == 0 || idx < 0 || idx >= n) ? -1 : idx; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_data_field(DESCR_t v, int field_i, DESCR_t *out) {
    if (field_i == 0 || !IS_DATA_INST_fn(v) || !v.u) return 0;
    DATINST_t *ia = (DATINST_t *)v.u;
    if (!ia->type || !ia->type->name || !ia->fields) return 0;
    DESCR_t *arr = (DESCR_t *)0;
    int n = 0;
    if (rt_lv_is_list(v, &arr, &n)) { int idx = sort_field_slot(field_i, n); if (!arr || idx < 0) return 0; *out = arr[idx]; return 1; }
    { int idx = sort_field_slot(field_i, ia->type->nfields); if (idx < 0) return 0; *out = ia->fields[idx]; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_field_cmp(DESCR_t a, DESCR_t b, int field_i, int num) {
    int ra = sort_type_rank(a, num), rb = sort_type_rank(b, num);
    if (ra != rb) return ra < rb ? -1 : 1;
    {
        DESCR_t fa, fb;
        int ha = sort_data_field(a, field_i, &fa), hb = sort_data_field(b, field_i, &fb), c;
        if (!ha && !hb) return sort_descr_cmp(a, b, num);
        if (!ha) return -1;
        if (!hb) return 1;
        c = sort_descr_cmp(fa, fb, num);
        return c ? c : sort_descr_cmp(a, b, num);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_descr(DESCR_t *a, DESCR_t *tmp, int n, int field_i, int num) {
    if (n < 2) return;
    {
        int m = n / 2, i = 0, j = n / 2, k = 0, t;
        sort_msort_descr(a, tmp, m, field_i, num);
        sort_msort_descr(a + m, tmp + m, n - m, field_i, num);
        while (i < m && j < n) { if (sort_field_cmp(a[j], a[i], field_i, num) < 0) tmp[k++] = a[j++]; else tmp[k++] = a[i++]; }
        while (i < m) tmp[k++] = a[i++];
        while (j < n) tmp[k++] = a[j++];
        for (t = 0; t < n; t++) a[t] = tmp[t];
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_pairs(TBPAIR_t **a, TBPAIR_t **tmp, int n, int by_val, int num) {
    if (n < 2) return;
    {
        int m = n / 2, i = 0, j = n / 2, k = 0, t;
        sort_msort_pairs(a, tmp, m, by_val, num);
        sort_msort_pairs(a + m, tmp + m, n - m, by_val, num);
        while (i < m && j < n) {
            DESCR_t x = by_val ? a[i]->val : a[i]->key_descr, y = by_val ? a[j]->val : a[j]->key_descr;
            if (sort_descr_cmp(y, x, num) < 0) tmp[k++] = a[j++];
            else tmp[k++] = a[i++];
        }
        while (i < m) tmp[k++] = a[i++];
        while (j < n) tmp[k++] = a[j++];
        for (t = 0; t < n; t++) a[t] = tmp[t];
    }
}
int sort_family(int bid, DESCR_t *args, int nargs, DESCR_t *out, int num) {
    if ((bid == BID_sort) && nargs >= 1 && args[0].v == DT_T && args[0].tbl) {
        TBBLK_t *tb = args[0].tbl;
        int i_mode = (nargs >= 2) ? (int)to_int(args[1]) : 1;
        if (i_mode < 1 || i_mode > 4) i_mode = 1;
        int n = 0;
        { TBPAIR_t *e; TBL_FOREACH(tb, e) n++; }
        TBPAIR_t **ent = rt_pvec_alloc((n>0?n:1));
        { int _k = 0; TBPAIR_t *e; TBL_FOREACH(tb, e) ent[_k++] = e; }
        int by_val = (i_mode % 2 == 0);
        { TBPAIR_t **_tmp = rt_pvec_alloc((n>0?n:1)); sort_msort_pairs(ent, _tmp, n, by_val, num); }
        extern DESCR_t rt_make_list(DESCR_t *a, int nn);
        if (tb->is_set) { DESCR_t *mem = rt_ws_alloc_descr((size_t)(n>0?n:1)); for (int _k = 0; _k < n; _k++) mem[_k] = ent[_k]->key_descr; *out = rt_make_list(mem, n); return 1; }
        if (i_mode >= 3) {
            DESCR_t *flat = rt_ws_alloc_descr((size_t)(2*n>0?2*n:1));
            for (int _k = 0; _k < n; _k++) { flat[2*_k] = ent[_k]->key_descr; flat[2*_k+1] = ent[_k]->val; }
            *out = rt_make_list(flat, 2*n);
            return 1;
        }
        DESCR_t outer = rt_make_list(NULL, 0);
        DESCR_t *pairs = rt_ws_alloc_descr((size_t)(n>0?n:1));
        for (int _k = 0; _k < n; _k++) { DESCR_t pv[2] = { ent[_k]->key_descr, ent[_k]->val }; pairs[_k] = rt_make_list(pv, 2); }
        rt_lv_set_elems(outer, DATA_ELEMS(pairs));
        rt_lv_set_size(outer, n);
        rt_lv_set_cap(outer, n);
        *out = outer;
        return 1;
    }
    if ((bid == BID_sortf) && (nargs == 1 || nargs == 2) && args[0].v == DT_T && args[0].tbl && args[0].tbl->is_set) {
        TBBLK_t *tb = args[0].tbl;
        int n = 0;
        { TBPAIR_t *e; TBL_FOREACH(tb, e) n++; }
        DESCR_t *mem = rt_ws_alloc_descr((size_t)(n>0?n:1));
        { int _k = 0; TBPAIR_t *e; TBL_FOREACH(tb, e) mem[_k++] = e->key_descr; }
        { DESCR_t *_tmp = rt_ws_alloc_descr((size_t)(n>0?n:1)); sort_msort_descr(mem, _tmp, n, sortf_field_i(args, nargs), num); }
        extern DESCR_t rt_make_list(DESCR_t *a, int nn);
        *out = rt_make_list(mem, n);
        return 1;
    }
    if (((bid == BID_sortf) || (bid == BID_sort)) && (nargs == 1 || nargs == 2) && IS_DATA_INST_fn(args[0]) && args[0].u && sort_is_record(args[0])) {
        DATINST_t *di = (DATINST_t *)args[0].u;
        int n = di->type->nfields;
        if (n < 0) n = 0;
        DESCR_t *mem = rt_ws_alloc_descr((size_t)(n>0?n:1));
        for (int _k = 0; _k < n; _k++) mem[_k] = di->fields[_k];
        { DESCR_t *_tmp = rt_ws_alloc_descr((size_t)(n>0?n:1)); sort_msort_descr(mem, _tmp, n, (bid == BID_sortf) ? sortf_field_i(args, nargs) : 0, num); }
        extern DESCR_t rt_make_list(DESCR_t *a, int nn);
        *out = rt_make_list(mem, n);
        return 1;
    }
    if (((bid == BID_sort)&&(nargs==1||nargs==2))||((bid == BID_sortf)&&(nargs==1||nargs==2))) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t *arr=NULL;
        int n=0;
        if (!rt_lv_is_list(ld,&arr,&n)) return 0;
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
        if (n<=0) { *out=rt_make_list(NULL,0); return 1; }
        if(!arr) { *out=rt_make_list(NULL,0); return 1; }
        DESCR_t *sorted=rt_ws_alloc_descr((size_t)n);
        memcpy(sorted,arr,n*sizeof(DESCR_t));
        int field_i=(bid == BID_sortf)?sortf_field_i(args, nargs):0;
        { DESCR_t *_tmp = rt_ws_alloc_descr((size_t)(n>0?n:1)); sort_msort_descr(sorted, _tmp, n, field_i, num); }
        *out=rt_make_list(sorted,n);
        return 1;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) { return try_call_builtin_by_name_bl(fn, args, nargs, out, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_arg_is_list(DESCR_t v) { return rt_lv_is_list(v, (DESCR_t **)0, (int *)0); }
static int icn_scan_tail_gate(DESCR_t *args, int nargs, int subj, DESCR_t *out) {
    if (nargs > subj && !IS_FAIL_fn(args[subj]) && args[subj].v != DT_SNUL && !icn_cvt_chars_ok(args[subj])) return icn_argtype_raise(103, args[subj], out);
    for (int i = subj + 1; i < subj + 3 && i < nargs; i++) if (!IS_FAIL_fn(args[i]) && args[i].v != DT_SNUL && !icn_cvt_int_ok(args[i])) return icn_argtype_raise(101, args[i], out);
    return 0;
}
static int icn_math_arg(DESCR_t a, double *v, DESCR_t *out) {
    DESCR_t rv;
    if (!icn_cvt_num_ok(a)) return icn_argtype_raise(102, a, out);
    *v = relop_radix_coerce(a, &rv) ? to_real(rv) : to_real(a);
    return 0;
}
double icn_dtor(double x) { return x * 3.14159265358979323846 / 180.0; }
double icn_rtod(double x) { return x * 180.0 / 3.14159265358979323846; }
int icn_math1(double (*f)(double), int dom, DESCR_t a, DESCR_t *out) {
    double v;
    if (icn_math_arg(a, &v, out)) return 1;
    if ((dom == 1 && (v < -1.0 || v > 1.0)) || (dom == 3 && v < 0.0)) return icn_argtype_raise(205, REALVAL(v), out);
    errno = 0;
    double y = f(v);
    if (dom == 2 && errno == ERANGE) return icn_argtype_raise(204, FAILDESCR, out);
    *out = REALVAL(y);
    return 1;
}
int icn_math_log(DESCR_t *args, int nargs, DESCR_t *out) {
    double v, b;
    if (icn_math_arg(args[0], &v, out)) return 1;
    if (v <= 0.0) return icn_argtype_raise(205, REALVAL(v), out);
    if (nargs < 2 || args[1].v == DT_SNUL) { *out = REALVAL(log(v)); return 1; }
    if (icn_math_arg(args[1], &b, out)) return 1;
    if (b == 1.0 || b <= 0.0) return icn_argtype_raise(205, REALVAL(b), out);
    *out = REALVAL(log(v) / log(b));
    return 1;
}
static long icn_str_len_of(DESCR_t d) {
    char b[64];
    if (IS_INT_fn(d)) return (long)snprintf(b, sizeof b, "%lld", (long long)d.i);
    if (IS_REAL_fn(d)) { icon_real_str(d.r, b, sizeof b); return (long)strlen(b); }
    if (d.v == DT_BIG) return 2;
    if (IS_CSET_fn(d)) return (long)descr_slen(d);
    return d.s ? (long)descr_slen(d) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t icn_char_code(DESCR_t d) { long long rv; if (IS_INT_fn(d)) return d.i; if (d.v == DT_S && d.s && icon_radix_int(rt_cstr_d(d), &rv) > 0) return (int64_t)rv; return to_int(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_argtype_gate(int bid, DESCR_t *args, int nargs, DESCR_t *out, int strict) {
    if (strict && nargs == 0 && (bid == BID_char || bid == BID_icom)) return icn_argtype_raise(101, NULVCL, out);
    if (strict && nargs == 0 && bid == BID_ord) return icn_argtype_raise(103, NULVCL, out);
    if (nargs < 1 || IS_FAIL_fn(args[0])) return 0;
    switch (bid) {
        case BID_any:
        case BID_many:
        case BID_upto:
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(104, args[0], out);
        return icn_scan_tail_gate(args, nargs, 1, out);
        case BID_find:
        case BID_match:
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(103, args[0], out);
        return icn_scan_tail_gate(args, nargs, 1, out);
        case BID_trim:
        case BID_reverse:
        case BID_repl:
        return (icn_cvt_chars_ok(args[0]) || (bid == BID_reverse && icn_arg_is_list(args[0]))) ? 0 : icn_argtype_raise(103, args[0], out);
        case BID_detab:
        case BID_entab:
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(103, args[0], out);
        if (!strict) return 0;
        for (int j = 1; j < nargs; j++) {
            if (!icn_cvt_num_ok(args[j])) return icn_argtype_raise(101, args[j], out);
            if (j > 1 && to_int(args[j]) <= to_int(args[j - 1])) return icn_argtype_raise(210, INTVAL(to_int(args[j])), out);
        }
        return (nargs >= 2 && to_int(args[1]) <= 1) ? icn_argtype_raise(210, INTVAL(to_int(args[1])), out) : 0;
        case BID_map:
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(103, args[0], out);
        for (int i = 1; i < 3 && i < nargs; i++) if (!IS_FAIL_fn(args[i]) && args[i].v != DT_SNUL && !icn_cvt_chars_ok(args[i])) return icn_argtype_raise(103, args[i], out);
        if (!strict) return 0;
        {
            long l2 = (nargs >= 2 && !IS_FAIL_fn(args[1]) && args[1].v != DT_SNUL) ? icn_str_len_of(args[1]) : 26;
            long l3 = (nargs >= 3 && !IS_FAIL_fn(args[2]) && args[2].v != DT_SNUL) ? icn_str_len_of(args[2]) : 26;
            return l2 != l3 ? icn_argtype_raise(208, FAILDESCR, out) : 0;
        }
        case BID_char:
        if (!strict) return 0;
        if (!icn_cvt_num_ok(args[0])) return icn_argtype_raise(101, args[0], out);
        { int64_t c = icn_char_code(args[0]); return (c < 0 || c > 255) ? icn_argtype_raise(205, INTVAL(c), out) : 0; }
        case BID_ord:
        if (!strict) return 0;
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(103, args[0], out);
        return icn_str_len_of(args[0]) != 1 ? icn_argtype_raise(205, args[0], out) : 0;
        case BID_iand:
        case BID_ior:
        case BID_ixor:
        case BID_ishift:
        case BID_icom:
        if (!strict) return 0;
        for (int i = 0; i < (bid == BID_icom ? 1 : 2); i++) { DESCR_t a = i < nargs ? args[i] : NULVCL; if (IS_FAIL_fn(a) || !icn_cvt_num_ok(a)) return icn_argtype_raise(101, a, out); }
        return 0;
        case BID_left:
        case BID_right:
        case BID_center:
        if (!icn_cvt_chars_ok(args[0])) return icn_argtype_raise(103, args[0], out);
        if (nargs >= 2 && !IS_FAIL_fn(args[1]) && args[1].v != DT_SNUL && !icn_cvt_int_ok(args[1])) return icn_argtype_raise(101, args[1], out);
        if (nargs >= 2 && !IS_FAIL_fn(args[1]) && args[1].v != DT_SNUL && to_int(args[1]) < 0) return icn_argtype_raise(205, args[1], out);
        if (nargs >= 3 && !IS_FAIL_fn(args[2]) && args[2].v != DT_SNUL && !icn_cvt_chars_ok(args[2])) return icn_argtype_raise(103, args[2], out);
        return 0;
        case BID_abs:
        return (!strict || icn_cvt_num_ok(args[0])) ? 0 : icn_argtype_raise(102, args[0], out);
        case BID_atan:
        if (!strict) return 0;
        if (!icn_cvt_num_ok(args[0])) return icn_argtype_raise(102, args[0], out);
        if (nargs >= 2 && !IS_FAIL_fn(args[1]) && args[1].v != DT_SNUL && !icn_cvt_num_ok(args[1])) return icn_argtype_raise(102, args[1], out);
        return 0;
        case BID_tab:
        case BID_move:
        return icn_cvt_int_ok(args[0]) ? 0 : icn_argtype_raise(101, args[0], out);
        case BID_insert:
        case BID_member:
        return (args[0].v == DT_T || (bid == BID_insert && icn_arg_is_list(args[0]))) ? 0 : icn_argtype_raise(122, args[0], out);
        case BID_get:
        case BID_pop:
        case BID_pull:
        case BID_push:
        case BID_put:
        return icn_arg_is_list(args[0]) ? 0 : icn_argtype_raise(108, args[0], out);
        case BID_sort:
        if (nargs >= 1 && !(icn_arg_is_list(args[0]) || args[0].v == DT_T || (IS_DATA_INST_fn(args[0]) && args[0].u && args[0].u->type))) return icn_argtype_raise(115, args[0], out);
        return 0;
        case BID_delete:
        return (args[0].v == DT_T || IS_STR_fn(args[0]) || icn_arg_is_list(args[0])) ? 0 : icn_argtype_raise(122, args[0], out);
        case BID_key:
        return (args[0].v == DT_T && args[0].tbl && !args[0].tbl->is_set) ? 0 : icn_argtype_raise(124, args[0], out);
        case BID_read:
        case BID_reads:
        if (!(args[0].v == DT_SNUL || IS_FH_fn(args[0]))) return icn_argtype_raise(105, args[0], out);
        if (bid == BID_reads && nargs >= 2 && !IS_FAIL_fn(args[1]) && args[1].v != DT_SNUL) {
            if (!icn_cvt_int_ok(args[1])) return icn_argtype_raise(101, args[1], out);
            if (to_int(args[1]) < 1) return icn_argtype_raise(205, args[1], out);
        }
        return 0;
        case BID_list:
        if (nargs >= 1 && !IS_FAIL_fn(args[0]) && args[0].v != DT_SNUL) {
            if (!icn_cvt_int_ok(args[0])) return icn_argtype_raise(101, args[0], out);
            if (to_int(args[0]) < 0) return icn_argtype_raise(205, args[0], out);
        }
        return 0;
        case BID_sortf:
        if (nargs != 1 && nargs != 2) return 0;
        if (!(args[0].v == DT_DATA || (args[0].v == DT_T && args[0].tbl && args[0].tbl->is_set))) return icn_argtype_raise(125, args[0], out);
        return sortf_field_i(args, nargs) == 0 ? icn_argtype_raise(205, args[1], out) : 0;
        case BID_bal:
        for (int i = 0; i < 3 && i < nargs; i++) if (!IS_FAIL_fn(args[i]) && args[i].v != DT_SNUL && !icn_cvt_chars_ok(args[i])) return icn_argtype_raise(104, args[i], out);
        return icn_scan_tail_gate(args, nargs, 3, out);
        default:
        return 0;
    }
}
int try_call_builtin_by_name_eval_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    long erq[2] = { 0, RT_RQ_EVAL_ARM };
    int hit;
    if (!rq || rq[1] != RT_RQ_EVAL_ARM) return try_call_builtin_by_name(fn, args, nargs, out);
    hit = try_call_builtin_by_name_bl_s_rq(fn, args, nargs, out, -1, 0, erq);
    if (erq[0] == RT_TRY_EVAL_OPEN) { rq[0] = erq[0]; rq[1] = erq[1]; }
    return hit;
}
DESCR_t bt_ICNx24IMM(DESCR_t *args, int nargs) { if (nargs == 0) { extern int rt_icn_imm_assign(void); return rt_icn_imm_assign() ? NULVCL : FAILDESCR; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_ICNx24UDIGITS(DESCR_t *args, int nargs) {
    if (nargs == 1) { extern DESCR_t bt_ICNx24UDIGITS(DESCR_t *, int); return bt_ICNx24UDIGITS(args, nargs); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_ICNx24UIDENT(DESCR_t *args, int nargs) { if (nargs == 1) { extern DESCR_t bt_ICNx24UIDENT(DESCR_t *, int); return bt_ICNx24UIDENT(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_ICNx24UMUTEX(DESCR_t *args, int nargs) { if (nargs <= 1) { extern DESCR_t bt_ICNx24UMUTEX(DESCR_t *, int); return bt_ICNx24UMUTEX(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t usort_leaf(int bid, DESCR_t *args, int nargs) {
    DESCR_t out = FAILDESCR;
    if (nargs < 1 || nargs > 2) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    if (icn_argtype_gate(bid, args, nargs, &out, 1)) return out;
    return sort_family(bid, args, nargs, &out, 1) > 0 ? out : (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_ICNx24USORT(DESCR_t *args, int nargs) { return usort_leaf(BID_sort, args, nargs); }
DESCR_t bt_ICNx24USORTF(DESCR_t *args, int nargs) { return usort_leaf(BID_sortf, args, nargs); }
static DESCR_t umaxmin_leaf(DESCR_t *args, int nargs, int dir) {
    DESCR_t *e = args, best;
    int n = nargs;
    if (nargs == 0) return FAILDESCR;
    if (nargs == 1 && args[0].v == DT_DATA && rt_lv_is_list(args[0], &e, &n) && (n <= 0 || !e)) return FAILDESCR;
    best = e[0];
    for (int i = 1; i < n; i++) if (sort_descr_cmp(e[i], best, 1) * dir > 0) best = e[i];
    return best;
}
DESCR_t bt_ICNx24UMAX(DESCR_t *args, int nargs) { return umaxmin_leaf(args, nargs, 1); }
DESCR_t bt_ICNx24UMIN(DESCR_t *args, int nargs) { return umaxmin_leaf(args, nargs, -1); }
