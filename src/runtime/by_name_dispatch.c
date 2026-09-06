#include "by_name_dispatch.h"
#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include "snobol4_system_fns.h"
#include "pl_arith_names.h"
int core_icn_error(int code, DESCR_t val);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) size_t sv_len(DESCR_t arg, const char *coerced) {
    if (arg.v == DT_S && arg.slen != 0xFFFFFFFFu) return arg.slen ? (size_t)arg.slen : (coerced ? strlen(coerced) : 0);
    return coerced ? strlen(coerced) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int icn_nxttab(int col, const int *stops, int nstops, int gap) {
    for (int k = 0; k < nstops; k++) if (stops[k] > col) return stops[k];
    int base = stops[nstops - 1]; int beyond = col - base;
    return base + ((beyond / gap) + 1) * gap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_cvt_chars_ok(DESCR_t d) { return d.v == DT_S || d.v == DT_I || d.v == DT_R || d.v == DT_C; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_cvt_int_ok(DESCR_t d) {
    if (IS_INT_fn(d) || IS_REAL_fn(d)) return 1;
    if (d.v == DT_S && d.s) { const char *p = d.s; while (*p == ' ') p++; if (*p == '+' || *p == '-') p++; int dg = 0; while (*p >= '0' && *p <= '9') { p++; dg = 1; } while (*p == ' ') p++; return dg && !*p; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_argtype_raise(int code, DESCR_t val, DESCR_t *out) { core_icn_error(code, val); *out = FAILDESCR; return 1; }
#include "rt/rt_arena.h"
#include "rt/gc_heap.h"
#include "builtins/gen_value.h"
#include "builtins/gen_runtime.h"
#include "../driver/driver_private.h"
#include "../parsers/raku/re.h"
#include "core.h"
#include "core/utf8.h"
#include "builtin_ids.h"
#include "pattern_match.h"
#include "rt/rt.h"
#include "rt/rt_list_view.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq")
        || !strcmp(name, "push") || !strcmp(name, "put");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int builtin_is_generator(const char *name) { return rt_builtin_is_generator(name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal")
        || !strcmp(name, "key") || !strcmp(name, "seq");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_known(const char *name)
{
    if (!name || !name[0]) return 0;
    static const char *const icn_known[] = {
        "integer", "real", "string", "numeric", "char", "cset", "abs", "ord",
        "iand", "ior", "ixor", "ishift", "icom", "proc",
        "sqrt", "sin", "cos", "tan", "atan", "log", "exp",
        "asin", "acos", "dtor", "rtod",
        "max", "min",
        "args", "collect", "copy",
        "image", "name", "type", "variable", "sort", "sortf",
        "center", "detab", "entab", "left", "map", "repl", "reverse", "right", "trim",
        "delete", "insert", "list", "member", "pull", "get", "pop",
        "push", "put", "set", "table",
        "close", "getenv", "open", "read", "reads",
        "seek", "where",
        "stop", "write", "writes",
        NULL
    };
    for (int i = 0; icn_known[i]; i++) if (!strcmp(icn_known[i], name)) return 1;
    return 0;
}
#include "../parsers/prolog/pl_cell.h"
#include "rt/rt_pl_trail.h"
static inline __attribute__((always_inline)) int plw_n_self(const DESCR_t *c) { return c->v == DT_N && c->slen == 1 && c->p == (void *)c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int plw_unbound_tag(const DESCR_t *c) { return c->v == DT_SNUL || c->v == DT_FAIL || (c->v == (DTYPE_t)DT_PLVAR && c->p == (void *)c) || plw_n_self(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_cell_deref_slow(DESCR_t *c) {
    DESCR_t *prev = (DESCR_t *)0;
    for (int guard = 0; guard < 4096; guard++) {
        if (c->v == DT_N && c->slen == 1 && c->p) { prev = c; c = (DESCR_t *)c->p; continue; }
        if (c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) { prev = c; c = ((VCELL_t *)c->p)->cellp; continue; }
        if (c->v == (DTYPE_t)DT_PLVAR && c->p && c->p != (void *)c) { prev = c; c = (DESCR_t *)c->p; continue; }
        return c;
    }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_cell_deref(DESCR_t *c) {
    if (c->v != DT_N && (c->v != (DTYPE_t)DT_PLVAR || !c->p || c->p == (void *)c)) return c;
    return plw_cell_deref_slow(c);
}
extern void *rt_plj_alloc(size_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_bind(DESCR_t *cell, DESCR_t word, pl_tr_ctx_t *cx) {
    char probe; char *floor_ = &probe;
    if (pl_tr_needs_log(cx, cell, floor_)) pl_tr_push(cx, cell);
    if ((char *)cell <= floor_) { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); *j = word; word.v = (DTYPE_t)DT_PLVAR; word.slen = 0; word.p = (void *)j; }
    *cell = word;
}
static int plw_vvb_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_NO_VVB"); p = (e && e[0] == '1') ? 0 : 1; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_unify_cells(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) {
    DESCR_t *A = plw_cell_deref(a), *B = plw_cell_deref(b);
    if (A == B) return 1;
    int av = plw_unbound_tag(A), bv = plw_unbound_tag(B);
    if (av && bv) {
        char probe; char *floor_ = &probe;
        if (plw_vvb_on() && (char *)A > floor_ && (char *)B > floor_) {
            DESCR_t *lo = A < B ? A : B; DESCR_t *hi = A < B ? B : A;
            DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)hi;
            if (hi->v != (DTYPE_t)DT_PLVAR || hi->p != (void *)hi) { DESCR_t u; u.v = (DTYPE_t)DT_PLVAR; u.slen = 0; u.p = (void *)hi; plw_bind(hi, u, cx); }
            plw_bind(lo, r, cx); return 1;
        }
        { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; plw_bind(A, r, cx); plw_bind(B, r, cx); return 1; } }
    if (av) { plw_bind(A, *B, cx); return 1; }
    if (bv) { plw_bind(B, *A, cx); return 1; }
    if (A->v == (DTYPE_t)DT_PLREF && B->v == (DTYPE_t)DT_PLREF) {
        if (A->slen != B->slen) return 0;
        int ar = (int)(A->slen & 0xFFFFu);
        DESCR_t *aa = (DESCR_t *)A->p, *bb = (DESCR_t *)B->p;
        for (int i = 0; i < ar; i++) if (!plw_unify_cells(&aa[i], &bb[i], cx)) return 0;
        return 1;
    }
    if (A->v == (DTYPE_t)DT_PLREF || B->v == (DTYPE_t)DT_PLREF) return 0;
    if (A->v == DT_I && B->v == DT_I && !A->slen && !B->slen) return A->i == B->i;
    if (((int)A->v == DT_S || (int)A->v == DT_A) && ((int)B->v == DT_S || (int)B->v == DT_A)) {
        extern const char *prolog_atom_name(int);
        const char *x = ((int)A->v == DT_S) ? (A->s ? A->s : "") : prolog_atom_name((int)A->i);
        const char *y = ((int)B->v == DT_S) ? (B->s ? B->s : "") : prolog_atom_name((int)B->i);
        return x && y && strcmp(x, y) == 0;
    }
    { extern int rt_descr_equal(DESCR_t, DESCR_t); return rt_descr_equal(*A, *B); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_entry(DESCR_t *tmp) {
    if (tmp->v == DT_N && tmp->slen == 1 && tmp->p) return (DESCR_t *)tmp->p;
    if (tmp->v == DT_N && tmp->slen == 2 && tmp->p && ((VCELL_t *)tmp->p)->cellp) return ((VCELL_t *)tmp->p)->cellp;
    return tmp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) int plw_unify_vals(DESCR_t va, DESCR_t vb, pl_tr_ctx_t *cx) {
    DESCR_t ta = va, tb = vb;
    return plw_unify_cells(plw_entry(&ta), plw_entry(&tb), cx);
}
__attribute__((visibility("hidden"))) int plw_unify_cells_x(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) { return plw_unify_cells(a, b, cx); }
__attribute__((visibility("hidden"))) int plw_unify_cell_val(DESCR_t *dst, DESCR_t val, pl_tr_ctx_t *cx) { DESCR_t tmp = val; return plw_unify_cells(dst, plw_entry(&tmp), cx); }
__attribute__((visibility("hidden"))) void plw_bind_x(DESCR_t *cell, DESCR_t word, pl_tr_ctx_t *cx) { plw_bind(cell, word, cx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_catch_handle_c(DESCR_t *a, int n, void *ball, pl_tr_ctx_t *cx)
{
    if (!a || n < 1 || !ball) return 0;
    return plw_unify_cells(plw_entry(&a[0]), (DESCR_t *)ball, cx) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_det_cell(DESCR_t *tmp) {
    { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
    DESCR_t *c = plw_cell_deref(plw_entry(tmp));
    if (c->v == DT_SNUL || c->v == DT_FAIL) { c->v = (DTYPE_t)DT_PLVAR; c->slen = 0; c->p = (void *)c; }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_deref_val(DESCR_t v) { DESCR_t t = v; return *plw_cell_deref(plw_entry(&t)); }
DESCR_t rt_pl_fresh_var_ref(void) { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name)
{
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name))  return 0;
    if (!strncmp(name, "__rk_test_", 10)) return 1;
    static const char *const known[] = {
        "write", "writes", "stop",
        "integer", "real", "string", "numeric", "char", "chr", "ord", "cset",
        "type", "image", "proc", "args", "copy",
        "abs", "sqrt", "sin", "cos", "tan", "exp", "log",
        "asin", "acos", "atan", "dtor", "rtod",
        "max", "min",
        "trim", "reverse", "repl", "map", "left", "center", "right",
        "detab", "entab", "read", "reads",
        "sprintf", "printf",
        "iand", "ior", "ixor", "ishift", "icom",
        "table", "list", "set", "sort", "sortf", "get", "pop", "pull",
        "member", "insert", "delete", "key",
        "[]",
        "__apply__",
        "MAKELIST",
        "__rk_arr", "__rk_arr_lit", "__rk_arr_lit_item", "arr_get", "arr_set_pure", "arr_init", "arr_last", "array_sort", "array_reverse", "arr_make",
        "__rk_arr_xx", "__rk_arr_at", "__rk_arr_sort", "__rk_arr_min", "__rk_arr_max", "__rk_arr_first",
        "__rk_arr_keys", "__rk_arr_values", "__rk_range_arr", "__rk_arr_slice", "__rk_arr_pick",
        "__rk_reduce_add", "__rk_reduce_sub", "__rk_reduce_mul", "__rk_reduce_cat", "__rk_reduce_min", "__rk_reduce_max",
        "__rk_div", "rk_write", "rk_writes", "rk_write_arr", "__rk_named_call", "__rk_rep", "__rk_exit",
        "__pas_ca_pack", "__pas_ca_unpack",
        "__rk_hash",
        "elems", "push_pure", "unshift_pure", "arr_tail",
        "hash_get", "hash_set_pure", "hash_delete_pure", "hash_exists",
        "hash_keys", "hash_values", "hash_pairs", "hash_kv",
        "__rk_jct_any", "__rk_jct_all", "__rk_jct_one", "__rk_jct_none",
        "obj_new", "meth_call", "field_set", "field_set_pub", "field_get_pub",
        "die", "script_die", "srand",
        "callsame", "nextsame", "callwith",
        "__multi_call", "__param_check", "__blk_ref", "__blk_invoke",
        "TIME", "DATE",
        "IDENTICAL", "getenv", "open", "where", "close", "collect", "seek",
        "LT", "LE", "GT", "GE", "EQ", "NE", "LGT", "LLT", "LGE", "LLE", "LEQ", "LNE",
        "IDENT", "DIFFER", "SIZE", "TRIM", "DUPL", "REPLACE", "REMDR", "SNO$NAME",
        "SUBSTR", "REVERSE", "LPAD", "RPAD", "INTEGER", "DATATYPE",
        "ARRAY", "TABLE", "ITEM", "PROTOTYPE", "CONVERT", "DATA", "APPLY", "OPSYN", "VALUE", "SNO$KWSET", "SNO$NRET", "SNO$WANTNM",
        "EVAL", "SNO$MKEXPR", "SNO$MKPAT", "SNO$STMT",
        "$unify", "$unify_lst", "$ix_g",
        NULL
    };
    for (int i = 0; known[i]; i++) if (!strcmp(known[i], name)) return 1;
    {
        if (dat_find_type(name)) return 1;
    }
    { extern int rt_dat_field_of_any(const char *); if (rt_dat_field_of_any(name)) return 1; }
    { extern const char *rt_builtin_synonym(const char *); if (rt_builtin_synonym(name)) return 1; }
    return 0;
}
#define SOH '\x01'
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *itos(long long v, char *buf, size_t cap) {
    if (cap < 2) { buf[0] = '\0'; return buf; }
    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    char tmp[24]; int n = 0; int neg = (v < 0);
    unsigned long long u = neg ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
    while (u) { tmp[n++] = (char)('0' + (int)(u % 10)); u /= 10; }
    if (neg) tmp[n++] = '-';
    size_t len = (size_t)n; if (len >= cap) len = cap - 1;
    for (size_t i = 0; i < len; i++) buf[i] = tmp[len - 1 - i];
    buf[len] = '\0'; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rtos(double r, char *buf, size_t cap) {
    gcvt(r, 14, buf); (void)cap; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *to_cstring(DESCR_t v, char *scratch, size_t scap) {
    if (IS_INT_fn(v))  { return itos((long long)v.i, scratch, scap); }
    if (IS_REAL_fn(v)) { return rtos(v.r, scratch, scap); }
    const char *s = VARVAL_fn(v); return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icon_radix_int(const char *s, long long *out) {
    const char *p = s; while (*p == ' ' || *p == '\t') p++;
    int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
    int base = 0; const char *bstart = p;
    while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
    if (!(p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36)) return 0;
    p++; const char *dstart = p; long long v = 0;
    while (*p) {
        int d = -1;
        if (*p >= '0' && *p <= '9') d = *p - '0';
        else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
        if (d < 0 || d >= base) break;
        v = v * base + d; p++;
    }
    while (*p == ' ' || *p == '\t') p++;
    if (!(p > dstart && *p == '\0')) return 0;
    *out = neg ? -v : v; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t elem_to_descr(const char *s, size_t slen) {
    char *buf = rt_ws_alloc(slen + 1);
    memcpy(buf, s, slen); buf[slen] = '\0';
    char *ep; long iv = strtol(buf, &ep, 10);
    if (*ep == '\0' && ep > buf) return INTVAL(iv);
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_is(DESCR_t v) {
    return (IS_STR_fn(v) || v.v == DT_SNUL) && v.s && v.s[0] == '\x03';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_num(double a, double b, int op) {
    switch (op) { case TT_EQ: return a == b; case TT_NE: return a != b; case TT_LT: return a < b;
                  case TT_LE: return a <= b; case TT_GT: return a > b; case TT_GE: return a >= b;
                  default: return a == b; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_str(const char *a, const char *b, int op) {
    int c = strcmp(a, b);
    switch (op) { case TT_EQ: return c == 0; case TT_NE: return c != 0; case TT_LT: return c < 0;
                  case TT_LE: return c <= 0; case TT_GT: return c > 0; case TT_GE: return c >= 0;
                  default: return c == 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric) {
    const char *s = jct.s; if (!s || s[0] != '\x03' || !s[1]) return 0;
    char flav = s[1];
    double sx = numeric ? to_real(scalar) : 0.0;
    const char *sstr = numeric ? NULL : VARVAL_fn(scalar); if (!sstr) sstr = "";
    int total = 0, hits = 0;
    const char *p = s + 2;
    while (*p == SOH) {
        p++; const char *start = p;
        int hit;
        if (*p == '\x03') {
            int depth = 1; p++;
            while (*p && depth > 0) { if (*p == '\x03') depth++; else if (*p == '\x04') depth--; p++; }
            size_t L = (size_t)(p - start);
            char *mb = rt_ws_alloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
            hit = junction_collapse(scalar, STRVAL(mb), op, numeric);
        } else {
            while (*p && *p != SOH && *p != '\x04') p++;
            size_t L = (size_t)(p - start);
            char mb[64]; size_t cl = L < 63 ? L : 63; memcpy(mb, start, cl); mb[cl] = '\0';
            hit = numeric ? jct_one_cmp_num(sx, strtod(mb, NULL), op) : jct_one_cmp_str(sstr, mb, op);
        }
        total++; if (hit) hits++;
    }
    switch (flav) { case 'a': return hits >= 1; case 'l': return total > 0 && hits == total;
                    case 'o': return hits == 1; case 'n': return hits == 0; default: return 0; }
}
#define GRAMMAR_MAX 128
#define RK_NAMED_MAX 32
static struct { const char *qname; const char *body; int flavor; } gram_reg[GRAMMAR_MAX];
static int gram_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_set(const char *qname, const char *body, int flavor) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) { gram_reg[i].body = rt_ws_strdup(body); gram_reg[i].flavor = flavor; return; }
    if (gram_n < GRAMMAR_MAX) { gram_reg[gram_n].qname = rt_ws_strdup(qname); gram_reg[gram_n].body = rt_ws_strdup(body); gram_reg[gram_n].flavor = flavor; gram_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gram_get_flavor(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].flavor;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gram_get(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].body;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_grammar_builtin_class(const char *nm) {
    if (!nm) return NULL;
    if (!strcmp(nm, "digit"))  return "[0-9]";
    if (!strcmp(nm, "alpha"))  return "[a-zA-Z]";
    if (!strcmp(nm, "alnum"))  return "[a-zA-Z0-9]";
    if (!strcmp(nm, "upper"))  return "[A-Z]";
    if (!strcmp(nm, "lower"))  return "[a-z]";
    if (!strcmp(nm, "space"))  return "[ \\t\\n\\r]";
    if (!strcmp(nm, "ws"))     return "[ \\t\\n\\r]*";
    if (!strcmp(nm, "xdigit")) return "[0-9a-fA-F]";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_expand(const char *gname, const char *body, int flavor, char *out, int outsz, int depth) {
    int op = 0, n = (int)strlen(body);
    for (int i = 0; i < n && op < outsz - 1; ) {
        char c = body[i];
        if (c == '\\' && i + 1 < n) { out[op++] = c; if (op < outsz - 1) out[op++] = body[i + 1]; i += 2; continue; }
        if (c == '\'' || c == '"') {
            char q = c; i++;
            while (i < n && body[i] != q && op < outsz - 2) {
                if (body[i] == '\\' && i + 1 < n) { out[op++] = body[i]; out[op++] = body[i+1]; i += 2; continue; }
                char lc = body[i++];
                if (strchr(".^$*+?()[]{}|\\", lc)) out[op++] = '\\';
                out[op++] = lc;
            }
            if (i < n) i++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            while (i < n && (body[i]==' '||body[i]=='\t'||body[i]=='\n'||body[i]=='\r')) i++;
            if (flavor == 1 && op < outsz - 3) { out[op++]='\\'; out[op++]='s'; out[op++]='*'; }
            continue;
        }
        if (c == '<' && i + 1 < n) {
            int ns = i + 1;
            if (body[ns] == '.' && ns + 1 < n && isalpha((unsigned char)body[ns + 1])) ns++;
            if (isalpha((unsigned char)body[ns])) {
                int j = ns; char nm[64]; int nl = 0;
                while (j < n && (isalnum((unsigned char)body[j]) || body[j] == '_') && nl < 63) nm[nl++] = body[j++];
                nm[nl] = '\0';
                if (j < n && body[j] == '>' && !(j + 1 < n && body[j + 1] == '(')) {
                    const char *sub = NULL; int subfl = 0;
                    if (depth < 16) { char qn[256]; snprintf(qn, sizeof qn, "%s::%s", gname, nm); sub = gram_get(qn); subfl = gram_get_flavor(qn); }
                    if (sub) {
                        char tmp[4096]; gram_expand(gname, sub, subfl, tmp, sizeof tmp, depth + 1);
                        if (op < outsz - 1) out[op++] = '(';
                        for (int k = 0; tmp[k] && op < outsz - 1; k++) out[op++] = tmp[k];
                        if (op < outsz - 1) out[op++] = ')';
                        i = j + 1; continue;
                    }
                    const char *bc = rk_grammar_builtin_class(nm);
                    if (bc) {
                        for (int k = 0; bc[k] && op < outsz - 1; k++) out[op++] = bc[k];
                        i = j + 1; continue;
                    }
                }
            }
        }
        out[op++] = c; i++;
    }
    out[op] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_grammar_register(const char *qname, const char *body, int flavor) { if (qname && body) gram_set(qname, body, flavor); }
int rt_grammar_count(void) { return gram_n; }
const char *rt_grammar_qname(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].qname : NULL; }
const char *rt_grammar_body(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].body : NULL; }
int rt_grammar_flavor(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].flavor : 0; }
int rt_grammar_has_top(const char *gname) { if (!gname) return 0; char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname); return gram_get(qn) != NULL; }
extern DESCR_t rk_gram_enter_box(bb_box_fn fn, const char *sigma, long delta, void *zeta, long *out_delta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_run_native(bb_box_fn bf, const char *subj, DESCR_t *out) {
    long Delta = (long)strlen(subj); long final_delta = 0; char fb[256] __attribute__((aligned(16))); memset(fb, 0, sizeof fb);
    DESCR_t r = rk_gram_enter_box(bf, subj, Delta, (void *)fb, &final_delta);
    int matched = (r.v != DT_FAIL); int full = matched && (final_delta == Delta);
    *out = full ? STRVAL(rt_ws_strdup_c(subj)) : NULVCL; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int grammar_parse_core(const char *gname, const char *subj, DESCR_t *out) {
    if (!gname) gname = ""; if (!subj) subj = "";
    { char gpn[320]; snprintf(gpn, sizeof gpn, "gram__%s__TOP", gname);
      extern void *rt_proc_get_fn(const char *); bb_box_fn bf = (bb_box_fn)rt_proc_get_fn(gpn); if (bf) return rk_gram_run_native(bf, subj, out); }
    char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname);
    const char *body = gram_get(qn);
    if (!body) { *out = FAILDESCR; return 1; }
    int topflv = gram_get_flavor(qn);
    char pat[4096]; gram_expand(gname, body, topflv, pat, sizeof pat, 0);
    Nfa *nfa = nfa_build(pat);
    if (!nfa) { *out = FAILDESCR; return 1; }
    Match m; nfa_exec(nfa, subj, &m);
    nfa_free(nfa);
    int slen = (int)strlen(subj);
    int ok = m.matched && m.full_start == 0 && m.full_end == slen;
    *out = ok ? STRVAL(rt_ws_strdup_c(subj)) : NULVCL; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    if (!meth || !*meth) return 0;
    char sb[64]; const char *s = to_cstring(recv, sb, sizeof sb); if (!s) s = ""; size_t n = strlen(s);
    if (!strcmp(meth, "chars")) { *out = INTVAL((long)utf8_strlen(s)); return 1; }
    if (!strcmp(meth, "uc")) { char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)toupper((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "lc") || !strcmp(meth, "fc")) {
        char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "tc")) { char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); if (n > 0) r[0] = (char)toupper((unsigned char)r[0]); *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "tclc")) {
        char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; if (n > 0) r[0] = (char)toupper((unsigned char)r[0]);
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "flip")) { char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = s[n - 1 - i]; r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "trim")) {
        size_t a = 0, b = n; while (a < b && isspace((unsigned char)s[a])) a++; while (b > a && isspace((unsigned char)s[b - 1])) b--; char *r = (char *)rt_ws_alloc(b - a + 1);
        memcpy(r, s + a, b - a); r[b - a] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "Str")) { *out = STRVAL(rt_ws_strdup_c(s)); return 1; }
    if (!strcmp(meth, "Int")) { if (IS_INT_fn(recv)) { *out = recv; return 1; } if (IS_REAL_fn(recv)) { *out = INTVAL((long)recv.r); return 1; } *out = INTVAL((long)atoll(s)); return 1; }
    if (!strcmp(meth, "contains") && nmargs >= 1) { char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; *out = INTVAL(strstr(s, nd) ? 1 : 0); return 1; }
    if (!strcmp(meth, "starts-with") && nmargs >= 1) { char nb[128]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; size_t ln = strlen(nd); *out = INTVAL((ln <= n && !strncmp(s, nd, ln)) ? 1 : 0); return 1; }
    if (!strcmp(meth, "ends-with") && nmargs >= 1) { char nb[128]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; size_t ln = strlen(nd); *out = INTVAL((ln <= n && !strncmp(s + (n - ln), nd, ln)) ? 1 : 0); return 1; }
    if (!strcmp(meth, "subst") && nmargs >= 1) {
        char nb[256]; const char *needle = to_cstring(margs[0], nb, sizeof nb); if (!needle) needle = "";
        char rb[256]; const char *repl = (nmargs >= 2) ? to_cstring(margs[1], rb, sizeof rb) : ""; if (!repl) repl = "";
        size_t nl = strlen(needle), rl = strlen(repl);
        const char *hit = (nl > 0) ? strstr(s, needle) : NULL;
        if (!hit) { char *o = (char *)rt_ws_alloc(n + 1); memcpy(o, s, n + 1); *out = STRVAL(o); return 1; }
        size_t pre = (size_t)(hit - s); char *o = (char *)rt_ws_alloc(n - nl + rl + 1);
        memcpy(o, s, pre); memcpy(o + pre, repl, rl); memcpy(o + pre + rl, hit + nl, n - pre - nl); o[n - nl + rl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(meth, "index") && nmargs >= 1) {
        char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; const char *hit = strstr(s, nd); *out = hit ? INTVAL((long)(hit - s)) : NULVCL; return 1;
    }
    if (!strcmp(meth, "substr") && nmargs >= 1) {
        long from = IS_INT_fn(margs[0]) ? (long)margs[0].i : atol(to_cstring(margs[0], sb, sizeof sb));
        long ln = (nmargs >= 2) ? (IS_INT_fn(margs[1]) ? (long)margs[1].i : atol(to_cstring(margs[1], sb, sizeof sb))) : (long)utf8_strlen(s) - from; if (from < 0) from = 0; if (ln < 0) ln = 0;
        long avail = (long)utf8_strlen(s) - from; if (avail < 0) avail = 0; if (ln > avail) ln = avail;
        if (ln == 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        *out = SUBSTR_fn(recv, INTVAL(from + 1), INTVAL(ln)); return 1;
    }
    if (!strcmp(meth, "chr")) { long cp = IS_INT_fn(recv) ? (long)recv.i : (long)atoll(s); char *r = (char *)rt_ws_alloc(2); r[0] = (char)(cp & 0xFF); r[1] = '\0'; *out = BSTRVAL(r, 1); return 1; }
    if (!strcmp(meth, "ord")) {
        size_t on = descr_slen(recv);
        if (!s || !on) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]); return 1;
    }
    if (!strcmp(meth, "abs")) { if (IS_INT_fn(recv)) { long v = (long)recv.i; *out = INTVAL(v < 0 ? -v : v); } else { *out = REALVAL(fabs(to_real(recv))); } return 1; }
    if (!strcmp(meth, "floor")) { *out = INTVAL((long)floor(to_real(recv))); return 1; }
    if (!strcmp(meth, "ceiling")) { *out = INTVAL((long)ceil(to_real(recv))); return 1; }
    if (!strcmp(meth, "round")) { *out = INTVAL((long)floor(to_real(recv) + 0.5)); return 1; }
    if (!strcmp(meth, "rand")) { *out = REALVAL((double)rand() / RAND_MAX * to_real(recv)); return 1; }
    if (!strcmp(meth, "Bool") || !strcmp(meth, "so") || !strcmp(meth, "not")) {
        int truthy; if (IS_INT_fn(recv)) truthy = (recv.i != 0); else if (IS_REAL_fn(recv)) truthy = (recv.r != 0.0); else truthy = (n > 0);
        *out = INTVAL(!strcmp(meth, "not") ? (truthy ? 0 : 1) : (truthy ? 1 : 0)); return 1;
    }
    if (!strcmp(meth, "defined")) { *out = INTVAL(recv.v != DT_SNUL ? 1 : 0); return 1; }
    if ((!strcmp(meth, "succ") || !strcmp(meth, "pred")) && (IS_INT_fn(recv) || IS_REAL_fn(recv))) {
        int d = !strcmp(meth, "succ") ? 1 : -1; if (IS_INT_fn(recv)) *out = INTVAL((long)recv.i + d); else *out = REALVAL(recv.r + d); return 1;
    }
    if (!strcmp(meth, "words")) {
        char *r = (char *)rt_ws_alloc(n + 1); int op = 0, first = 1; size_t i = 0;
        while (i < n) {
            while (i < n && isspace((unsigned char)s[i])) i++; if (i >= n) break; if (!first) r[op++] = SOH; first = 0; while (i < n && !isspace((unsigned char)s[i])) r[op++] = s[i++];
        } r[op] = '\0';
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "comb")) {
        char *r = (char *)rt_ws_alloc(2 * n + 1); int op = 0;
        for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; } r[op] = '\0'; *out = STRVAL(r);
        return 1;
    }
    if (!strcmp(meth, "split") && nmargs >= 1) {
        char sept[64]; const char *sep = to_cstring(margs[0], sept, sizeof sept); if (!sep) sep = ""; size_t sl = strlen(sep); char *r = (char *)rt_ws_alloc(2 * n + 2); int op = 0;
        if (sl == 0) {
            for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; }
        } else { const char *p = s, *hit;
        while ((hit = strstr(p, sep)) != NULL) { size_t sg = (size_t)(hit - p); memcpy(r + op, p, sg); op += (int)sg; r[op++] = SOH; p = hit + sl; } size_t tl = strlen(p); memcpy(r + op, p, tl);
        op += (int)tl; } r[op] = '\0';
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "chomp")) {
        size_t b = n; if (b > 0 && (s[b - 1] == '\n' || s[b - 1] == '\r')) b--; char *r = (char *)rt_ws_alloc(b + 1); memcpy(r, s, b); r[b] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "wordcase")) {
        char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); int start = 1;
        for (size_t i = 0; i < n; i++) { if (isspace((unsigned char)r[i])) start = 1; else { if (start) r[i] = (char)toupper((unsigned char)r[i]); start = 0; } } *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "lines")) {
        char *r = (char *)rt_ws_alloc(2 * n + 1); int op = 0, first = 1; size_t i = 0;
        while (i < n) { if (!first) r[op++] = SOH; first = 0; while (i < n && s[i] != '\n') { if (s[i] != '\r') r[op++] = s[i]; i++; } if (i < n) i++; } r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "elems")) { if (n == 0) { *out = INTVAL(0); return 1; } int c = 1; for (size_t i = 0; i < n; i++) if (s[i] == SOH) c++; *out = INTVAL(c); return 1; }
    if (!strcmp(meth, "trans") && nmargs >= 1) {
        const char *lst = VARVAL_fn(margs[0]); if (!lst) lst = "";
        int total = lst[0] ? 1 : 0; for (const char *p = lst; *p; p++) if (*p == SOH) total++;
        if (total < 2 || (total % 2) != 0) { char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); *out = STRVAL(r); return 1; }
        int half = total / 2; if (half > 128) half = 128;
        const char *from[128]; size_t fromlen[128]; const char *to[128]; size_t tolen[128];
        size_t maxto = 1; int idx = 0; const char *seg = lst;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (idx < half) { from[idx] = seg; fromlen[idx] = L; }
            else if (idx - half < 128) { to[idx - half] = seg; tolen[idx - half] = L; if (L > maxto) maxto = L; }
            idx++; if (!nx) break; seg = nx + 1;
        }
        char *r = (char *)rt_ws_alloc(n * maxto + n + 1); size_t op = 0, i = 0;
        while (i < n) {
            int matched = -1;
            for (int k = 0; k < half; k++) { if (fromlen[k] > 0 && i + fromlen[k] <= n && memcmp(s + i, from[k], fromlen[k]) == 0) { matched = k; break; } }
            if (matched >= 0) { memcpy(r + op, to[matched], tolen[matched]); op += tolen[matched]; i += fromlen[matched]; }
            else { r[op++] = s[i++]; }
        }
        r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "join")) {
        const char *sep = ""; char jb[64]; if (nmargs >= 1) { sep = to_cstring(margs[0], jb, sizeof jb); if (!sep) sep = ""; } size_t sl = strlen(sep); int nsep = 0;
        for (size_t i = 0; i < n; i++) if (s[i] == SOH) nsep++; char *r = (char *)rt_ws_alloc(n + (size_t)nsep * sl + 1); int op = 0;
        for (size_t i = 0; i < n; i++) { if (s[i] == SOH) { memcpy(r + op, sep, sl); op += (int)sl; } else r[op++] = s[i]; } r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "fmt")) {
        extern void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
        char fb[256]; const char *fmt = (nmargs >= 1) ? to_cstring(margs[0], fb, sizeof fb) : "%s"; if (!fmt) fmt = "%s";
        DESCR_t fa[2]; fa[0] = margs ? margs[0] : NULVCL; fa[1] = recv;
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, fa, 2, 1, &r, &rl); *out = BSTRVAL(r, rl); return 1;
    }
    return 0;
}
static long g_pas_heap_ctr = 0;
static DESCR_t *g_pas_heap = (DESCR_t *)0;
static long g_pas_heap_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *pas_heap_cell(long n, int grow) {
    if (n <= 0) return (DESCR_t *)0;
    if (n >= g_pas_heap_cap) {
        if (!grow) return (DESCR_t *)0;
        long nc = g_pas_heap_cap ? g_pas_heap_cap : 1024;
        while (nc <= n) nc *= 2;
        DESCR_t *nh = (DESCR_t *)(g_pas_heap ? rt_ws_realloc(g_pas_heap, (size_t)nc * sizeof(DESCR_t)) : rt_ws_alloc((size_t)nc * sizeof(DESCR_t)));
        if (!nh) return (DESCR_t *)0;
        memset(nh + g_pas_heap_cap, 0, (size_t)(nc - g_pas_heap_cap) * sizeof(DESCR_t));
        g_pas_heap = nh; g_pas_heap_cap = nc;
    }
    return &g_pas_heap[n];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *resolve_method_chain(const char *cls, const char *mname, char *buf, int bufsz, int *pfound) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_roles(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (pfound) *pfound = -1;
    const char *chain[64]; int nchain = dat_mro(cls, chain, 64);
    if (nchain == 0) { chain[0] = cls; nchain = 1; }
    for (int ci = 0; ci < nchain; ci++) {
        snprintf(buf, bufsz, "%s__%s", chain[ci], mname);
        int found = rt_proc_has_native_fn(buf);
        if (!found) for (int pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { found = 1; break; }
        if (found) { if (pfound) *pfound = ci; return buf; }
        const char *roles[8]; int nr = dat_roles(chain[ci], roles, 8);
        for (int ri = 0; ri < nr; ri++) {
            snprintf(buf, bufsz, "%s__%s", roles[ri], mname);
            int rf = rt_proc_has_native_fn(buf);
            if (!rf) for (int pi = 0; pi < g_stage2.proc_count; pi++)
                if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { rf = 1; break; }
            if (rf) { if (pfound) *pfound = ci; return buf; }
        }
    }
    snprintf(buf, bufsz, "%s__%s", cls, mname);
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int meth_is_user_proc(const char *procname) {
    extern int rt_proc_has_native_fn(const char *name);
    if (procname && rt_proc_has_native_fn(procname)) return 1;
    if (procname) for (int pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, procname)) return 1;
    return 0;
}
typedef struct { DESCR_t self; char mname[128]; const char *mro[64]; int mro_len; int found_idx; DESCR_t args[16]; int nargs; } RedispFrame;
static RedispFrame g_redisp[64];
static int g_redisp_top = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t invoke_method_proc(const char *procname, DESCR_t *callargs, int total) {
    int pi;
    for (pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
    if (pi >= g_stage2.proc_count) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
        return rt_call_proc_descr(procname, total);
    }
    if (g_stage2.proc_table[pi].bb_idx >= 0) {
        extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); extern int rt_proc_has_native_fn(const char *name);
        if (rt_proc_has_native_fn(procname)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
            return rt_call_proc_descr(procname, total);
        }
        return ir_call_proc(pi, callargs, total);
    }
    return proc_table_call(pi, callargs, total);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_buildplan_tweak(const char *cname, DESCR_t self) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (!cname || !*cname) return;
    const char *chain[64]; int n = dat_mro(cname, chain, 64);
    if (n == 0) { chain[0] = cname; n = 1; }
    for (int i = n - 1; i >= 0; i--) {
        char proc[256]; snprintf(proc, sizeof proc, "%s__TWEAK", chain[i]);
        if (!meth_is_user_proc(proc)) continue;
        int pi; for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
        if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            g_call_args[0] = self; rt_call_proc_descr(proc, 1);
        } else {
            extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); DESCR_t a0 = self; ir_call_proc(pi, &a0, 1);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_build(const char *cname, DESCR_t self, DESCR_t *named, int nnamed) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_class_has_build(const char *cls);
    extern int dat_build_keys(const char *cls, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (!cname || !*cname) return;
    const char *chain[64]; int n = dat_mro(cname, chain, 64);
    if (n == 0) { chain[0] = cname; n = 1; }
    for (int i = n - 1; i >= 0; i--) {
        if (!dat_class_has_build(chain[i])) continue;
        char proc[256]; snprintf(proc, sizeof proc, "%s__BUILD", chain[i]);
        if (!meth_is_user_proc(proc)) continue;
        const char *keys[16]; int nk = dat_build_keys(chain[i], keys, 16);
        DESCR_t callargs[20]; callargs[0] = self;
        for (int k = 0; k < nk && k < 19; k++) { DESCR_t v = NULVCL;
            for (int ci = 0; ci + 1 < nnamed; ci += 2) { const char *kn = VARVAL_fn(named[ci]); if (kn && keys[k] && !strcmp(kn, keys[k])) { v = named[ci + 1]; break; } }
            callargs[1 + k] = v; }
        int total = 1 + nk;
        int pi; for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
        if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k]; rt_call_proc_descr(proc, total);
        } else {
            extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); ir_call_proc(pi, callargs, total);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_nrec_subrec_set(const char *cur, long fi, long ei, const char *val) {
    if (!cur) cur = ""; if (!val) val = ""; if (fi < 0) return rt_ws_strdup(cur); if (ei < 0) ei = 0;
    const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
    for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
    if (!fstart) return rt_ws_strdup(cur);
    size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
    char *field = rt_ws_alloc(flen + 1); memcpy(field, fstart, flen); field[flen] = '\0';
    long nsub = 1; for (size_t j = 0; j < flen; j++) if (field[j] == '\x05') nsub++;
    long want = (ei + 1 > nsub) ? ei + 1 : nsub;
    const char **elems = (const char **)rt_ws_alloc((size_t)want * sizeof(char *));
    const char *p = field; long ix = 0;
    for (;;) {
        const char *nx = strchr(p, '\x05'); size_t el = nx ? (size_t)(nx - p) : strlen(p); char *e = rt_ws_alloc(el + 1); memcpy(e, p, el); e[el] = '\0'; if (ix < want) elems[ix] = e; ix++;
        if (!nx) break; p = nx + 1;
    }
    for (long j = nsub; j < want; j++) elems[j] = "0";
    elems[ei] = val;
    size_t newflen = 0; for (long j = 0; j < want; j++) newflen += strlen(elems[j]); newflen += (size_t)(want - 1);
    char *newf = rt_ws_alloc(newflen + 1); size_t fp = 0;
    for (long j = 0; j < want; j++) { if (j) newf[fp++] = '\x05'; size_t L = strlen(elems[j]); memcpy(newf + fp, elems[j], L); fp += L; } newf[fp] = '\0';
    size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
    char *o = rt_ws_alloc(pre + fp + post + 1); memcpy(o, cur, pre); memcpy(o + pre, newf, fp); if (fend) memcpy(o + pre + fp, fend, post); o[pre + fp + post] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_mc_type_name(DESCR_t d) {
    switch (d.v) {
    case DT_I: return "Int";
    case DT_R: return "Num";
    case DT_S: return "Str";
    case DT_DATA: { if (d.u && d.u->type && d.u->type->name) return d.u->type->name; return "Any"; }
    default: return "Any";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_is_subtype(const char *a, const char *b) {
    if (!a || !b) return 0;
    if (!strcmp(b, "Any") || !strcmp(b, "Mu") || !strcmp(b, "Cool")) return 1;
    if (!strcmp(a, b)) return 1;
    int a_numleaf = (!strcmp(a, "Int") || !strcmp(a, "Num") || !strcmp(a, "Rat"));
    if ((!strcmp(b, "Numeric") || !strcmp(b, "Real")) && a_numleaf) return 1;
    { extern int dat_mro(const char *name, const char **out, int max); const char *mro[64]; int n = dat_mro(a, mro, 64);
      for (int i = 0; i < n; i++) if (mro[i] && !strcmp(mro[i], b)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_accepts(const char *ptype, DESCR_t arg) {
    if (!ptype || !strcmp(ptype, "Any") || !strcmp(ptype, "Mu")) return 1;
    int require_def = 0, require_undef = 0;
    const char *colon = strrchr(ptype, ':');
    const char *under = strrchr(ptype, '_');
    char base[64]; base[0] = 0;
    if (colon && (!strcmp(colon, ":D") || !strcmp(colon, ":U"))) {
        require_def   = !strcmp(colon, ":D");
        require_undef = !strcmp(colon, ":U");
        int bl = (int)(colon - ptype); if (bl > 63) bl = 63; memcpy(base, ptype, (size_t)bl); base[bl] = 0;
    } else if (under && (!strcmp(under, "_D") || !strcmp(under, "_U"))) {
        require_def   = !strcmp(under, "_D");
        require_undef = !strcmp(under, "_U");
        int bl = (int)(under - ptype); if (bl > 63) bl = 63; memcpy(base, ptype, (size_t)bl); base[bl] = 0;
    } else { snprintf(base, sizeof base, "%s", ptype); }
    int is_undef = (arg.v == DT_SNUL);
    if (require_def   && is_undef) return 0;
    if (require_undef) return is_undef;
    if (!strcmp(base, "Any") || !strcmp(base, "Mu") || !strcmp(base, "Cool")) return 1;
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
        char baseA[32], baseB[32];
        { int l = cA && (aD||aU) ? (int)(cA-A) : (int)strlen(A); if(l>31)l=31; memcpy(baseA,A,l); baseA[l]=0; }
        { int l = cB && (bD||bU) ? (int)(cB-B) : (int)strlen(B); if(l>31)l=31; memcpy(baseB,B,l); baseB[l]=0; }
        int same_base = !strcmp(baseA, baseB);
        if (same_base && (aD||aU) && !(bD||bU)) { narrower++; continue; }
        if (same_base && !(aD||aU) && (bD||bU)) { continue; }
        if (rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) narrower++;
        else if (!rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) tied++;
    }
    return narrower > 0 && (narrower + tied == na);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_multi_meth_dispatch(const char *cname, const char *mname, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!cname || !mname) return 0;
    int nm = nargs - 2; if (nm < 0) return 0; DESCR_t *ma = &args[2];
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_enum_count(void); extern const char *rt_proc_enum_name(int i);
    const char *mro[64]; int mn = dat_mro(cname, mro, 64); if (mn == 0) { mro[0] = cname; mn = 1; }
    static char acc_names[256][192]; static char acc_types[256][8][32]; int nacc = 0;
    int pcount = rt_proc_enum_count();
    for (int ci = 0; ci < mn && nacc < 256; ci++) {
        if (!mro[ci]) continue;
        char prefix[192]; int pl = snprintf(prefix, sizeof prefix, "%s__%s$", mro[ci], mname);
        for (int pi = 0; pi < pcount && nacc < 256; pi++) {
            const char *pn = rt_proc_enum_name(pi);
            if (!pn || strncmp(pn, prefix, (size_t)pl)) continue;
            const char *p = pn + pl; const char *e = strchr(p, '$');
            int arity = atoi(p); if (arity != nm) continue;
            int nt = 0; const char *q = e ? e + 1 : (const char *)0;
            while (q && nt < 8) {
                const char *nx = strchr(q, '$'); int len = nx ? (int)(nx - q) : (int)strlen(q); if (len > 31) len = 31; memcpy(acc_types[nacc][nt], q, (size_t)len); acc_types[nacc][nt][len] = 0;
                nt++; if (!nx) break; q = nx + 1;
            }
            int ok = 1; for (int i = 0; i < nm && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], ma[i])) { ok = 0; break; }
            if (!ok) continue;
            int dup = 0; for (int k = 0; k < nacc; k++) if (!strcmp(acc_names[k], pn)) { dup = 1; break; }
            if (dup) continue;
            snprintf(acc_names[nacc], sizeof acc_names[nacc], "%s", pn); nacc++;
        }
    }
    if (nacc == 0) return 0;
    int win = -1;
    for (int i = 0; i < nacc; i++) { int beaten = 0;
        for (int j = 0; j < nacc; j++) { if (i == j) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], nm)) { beaten = 1; break; } }
        if (!beaten) { win = i; break; } }
    if (win < 0) win = 0;
    int total = 1 + nm; DESCR_t *ca = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
    ca[0] = args[0]; for (int k = 0; k < nm; k++) ca[1 + k] = ma[k];
    *out = invoke_method_proc(acc_names[win], ca, total); return 1;
}
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs);
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen);
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen);
extern const char *icon_real_str(double r, char *buf, int bufsz);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_real_str(double r, char *buf, int bufsz) {
    if (isfinite(r) && r == floor(r) && fabs(r) < 1e15) { snprintf(buf, (size_t)bufsz, "%lld", (long long)r); return buf; }
    if (!isfinite(r)) return icon_real_str(r, buf, bufsz);
    for (int prec = 15; prec <= 17; prec++) { snprintf(buf, (size_t)bufsz, "%.*g", prec, r); if (strtod(buf, (char **)0) == r) return buf; }
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *icn_pad_str(DESCR_t d, char *buf, int bufsz) {
    if (IS_REAL_fn(d)) return icon_real_str(d.r, buf, bufsz);
    return VARVAL_fn(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_true_len(DESCR_t d, const char *materialized) { if (d.v == DT_S) return (int)descr_slen(d); return materialized ? (int)strlen(materialized) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * procval_name(DESCR_t v) {
    if (v.v != DT_E) return 0;
    if (v.slen == 0xFFFFFFFEu) return v.s;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].entry_pc == (int)v.i) return g_stage2.proc_table[i].name;
    return 0;
}
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { uint64_t magic; DESCR_t obj; int64_t idx; } ICN_OPGEN_t;
#define ICN_OPGEN_MAGIC 0x1CBA46E4E52DULL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_opgen_pump(ICN_OPGEN_t *g) {
    extern int list_bang_at(DESCR_t, int64_t, DESCR_t *);
    DESCR_t out;
    if (!g || !list_bang_at(g->obj, g->idx, &out)) return FAILDESCR;
    g->idx++;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n) {
    if (IS_INT_fn(callee)) { long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm) return FAILDESCR;
    if (rt_proc_is_registered(nm) || !strcmp(nm, "main")) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0};
        return rt_call_proc_descr(nm, n);
    }
    if (n == 1 && !strcmp(nm, "!")) { extern int list_bang_at(DESCR_t, int64_t, DESCR_t *); DESCR_t out; return list_bang_at(argv[0], 0, &out) ? out : FAILDESCR; }
    if (n == 1 && !strcmp(nm, "/")) return (argv[0].v == DT_SNUL || argv[0].v == 0) ? argv[0] : FAILDESCR;
    return rt_call_arr(nm, argv, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value_gen_h(DESCR_t callee, DESCR_t *argv, int n, void **hslot) {
    if (hslot) *hslot = (void *)0;
    if (IS_INT_fn(callee)) { long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm) return FAILDESCR;
    if (rt_proc_is_registered(nm)) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout);
        for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0};
        return rt_proc_call_gen_h(nm, n, hslot);
    }
    if (n == 1 && !strcmp(nm, "!")) {
        ICN_OPGEN_t *g = (ICN_OPGEN_t *)calloc(1, sizeof *g);
        if (!g) return FAILDESCR;
        g->magic = ICN_OPGEN_MAGIC; g->obj = argv[0]; g->idx = 0;
        DESCR_t first = icn_opgen_pump(g);
        if (IS_FAIL_fn(first) || !hslot) { free(g); return first; }
        *hslot = (void *)g;
        return first;
    }
    return rt_call_value(callee, argv, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_apply_gen_h(DESCR_t callee, DESCR_t lv, void **hslot) {
    DESCR_t buf[64]; int n = 0;
    if (lv.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(lv, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int ln = (int)FIELD_GET_fn(lv, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(lv, "frame_elems");
            DESCR_t *arr = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (ln > 64) ln = 64;
            if (arr) { for (int k = 0; k < ln; k++) buf[k] = arr[k]; n = ln; }
        } else { buf[0] = lv; n = 1; }
    } else { buf[0] = lv; n = 1; }
    return rt_call_value_gen_h(callee, buf, n, hslot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_call_value_spine_prep(DESCR_t callee, DESCR_t *argv, int n) {
    extern int rt_proc_jmp_entry(const char *name); extern void *rt_proc_fn(const char *name); extern long rt_proc_call_open(const char *name, int nargs);
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm || !rt_proc_is_registered(nm) || !rt_proc_jmp_entry(nm) || !rt_proc_is_generator(nm)) return (void *)0;
    { extern int rt_proc_gen_region_ft(const char *); if (rt_proc_gen_region_ft(nm) > 0) return (void *)0; }
    { extern DESCR_t g_call_args[]; for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0}; }
    if (!rt_proc_call_open(nm, n)) return (void *)0;
    return rt_proc_fn(nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_call_apply_spine_prep(DESCR_t callee, DESCR_t lv) {
    DESCR_t buf[64]; int n = 0;
    if (lv.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(lv, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int ln = (int)FIELD_GET_fn(lv, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(lv, "frame_elems");
            DESCR_t *arr = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (ln > 64) ln = 64;
            if (arr) { for (int k = 0; k < ln; k++) buf[k] = arr[k]; n = ln; }
        } else { buf[0] = lv; n = 1; }
    } else { buf[0] = lv; n = 1; }
    return rt_call_value_spine_prep(callee, buf, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_goal_key(DESCR_t goal, int extra, char *out, size_t cap, DESCR_t **args_out, int *ar_out) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(goal);
    const char *nm = (const char *)0; int ar = 0; DESCR_t *kids = (DESCR_t *)0;
    if ((int)v.v == DT_PLREF) { nm = prolog_atom_name((int)(v.slen >> 16)); ar = (int)(v.slen & 0xFFFFu); kids = (DESCR_t *)v.p; }
    else if ((int)v.v == (int)DT_A) nm = prolog_atom_name((int)v.i);
    else if ((int)v.v == (int)DT_S) nm = v.s;
    if (!nm || extra < 0) return 0;
    snprintf(out, cap, "%s/%d", nm, ar + extra);
    *args_out = kids; *ar_out = ar;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_goal_stage(DESCR_t *kids, int ar, DESCR_t *argv, int n) {
    extern DESCR_t g_call_args[];
    enum { CALL_ARGS_MAX = 64 };
    int k = 0;
    for (int i = 0; i < ar && k < CALL_ARGS_MAX; i++) g_call_args[k++] = kids[i];
    for (int i = 0; i < n && k < CALL_ARGS_MAX; i++) g_call_args[k++] = argv[i];
    for (int i = k; i < CALL_ARGS_MAX; i++) { DESCR_t z; memset(&z, 0, sizeof z); g_call_args[i] = z; }
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_goal_spine_prep(DESCR_t goal, DESCR_t *argv, int n) {
    extern int rt_proc_jmp_entry(const char *name); extern void *rt_proc_fn(const char *name); extern long rt_proc_call_open(const char *name, int nargs);
    char key[288]; DESCR_t *kids = (DESCR_t *)0; int ar = 0;
    if (!rt_pl_goal_key(goal, n, key, sizeof key, &kids, &ar)) return (void *)0;
    if (!rt_proc_is_registered(key) || !rt_proc_jmp_entry(key) || !rt_proc_is_generator(key)) return (void *)0;
    { extern int rt_proc_gen_region_ft(const char *); if (rt_proc_gen_region_ft(key) > 0) return (void *)0; }
    rt_pl_goal_stage(kids, ar, argv, n);
    if (!rt_proc_call_open(key, ar + n)) return (void *)0;
    return rt_proc_fn(key);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_goal_gen_h(DESCR_t goal, DESCR_t *argv, int n, void **hslot) {
    extern DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot);
    char key[288]; DESCR_t *kids = (DESCR_t *)0; int ar = 0;
    if (hslot) *hslot = (void *)0;
    if (!rt_pl_goal_key(goal, n, key, sizeof key, &kids, &ar)) { rt_pl_iso_throw_pi("type_error", "callable", "?", 0); return FAILDESCR; }
    if (!rt_proc_is_registered(key)) { rt_pl_iso_throw_existence_key(key); return FAILDESCR; }
    rt_pl_goal_stage(kids, ar, argv, n);
    return rt_proc_call_gen_h(key, ar + n, hslot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value_resume_h(void **hslot) {
    extern DESCR_t rt_proc_resume_frame_h(void **hslot);
    if (!hslot || !*hslot) return FAILDESCR;
    { ICN_OPGEN_t *g = (ICN_OPGEN_t *)*hslot;
      if (g->magic == ICN_OPGEN_MAGIC) { DESCR_t v = icn_opgen_pump(g); if (IS_FAIL_fn(v)) { free(g); *hslot = (void *)0; } return v; } }
    return rt_proc_resume_frame_h(hslot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_descr_equal(DESCR_t a, DESCR_t b) {
    if (a.v == DT_I && b.v == DT_I) return a.i == b.i;
    if (a.v == DT_R && b.v == DT_R) return a.r == b.r;
    if ((a.v == DT_S || a.v == DT_SNUL) && (b.v == DT_S || b.v == DT_SNUL)) { const char *x = a.s ? a.s : ""; const char *y = b.s ? b.s : ""; return strcmp(x, y) == 0; }
    { const char *x = VARVAL_fn(a); const char *y = VARVAL_fn(b); if (x && y) return strcmp(x, y) == 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_op_code(const char *s) {
    if (!strcmp(s, "add")) return BINOP_ADD; if (!strcmp(s, "sub")) return BINOP_SUB; if (!strcmp(s, "mul")) return BINOP_MUL;
    if (!strcmp(s, "div")) return BINOP_DIV; if (!strcmp(s, "idiv")) return BINOP_DIV; if (!strcmp(s, "mod")) return BINOP_MOD; if (!strcmp(s, "pow")) return BINOP_POW;
    return BINOP_ADD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_num_cmp(const char *s, DESCR_t a, DESCR_t b) {
    double av = IS_REAL_fn(a) ? a.r : (double)a.i; double bv = IS_REAL_fn(b) ? b.r : (double)b.i;
    if (!strcmp(s, "lt")) return av < bv; if (!strcmp(s, "gt")) return av > bv; if (!strcmp(s, "le")) return av <= bv;
    if (!strcmp(s, "ge")) return av >= bv; if (!strcmp(s, "eq")) return av == bv; if (!strcmp(s, "ne")) return av != bv;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_real_iso_str(double fv, char * fb, int bufsz) {
    for (int prec = 15; prec <= 17; prec++) { snprintf(fb, (size_t)bufsz, "%.*g", prec, fv); if (strtod(fb, (char **)0) == fv) break; }
    if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n + 2 < (size_t)bufsz) { fb[n] = '.'; fb[n + 1] = '0'; fb[n + 2] = '\0'; } }
    return fb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_arith2(const char *op, DESCR_t a, DESCR_t b) {
    extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    int ai = (a.v == DT_I), bi = (b.v == DT_I);
    if (ai && bi && !strcmp(op, "idiv")) { if (b.i == 0) return FAILDESCR; return INTVAL(a.i / b.i); }
    if (ai && bi && !strcmp(op, "div"))  { if (b.i == 0) return FAILDESCR; if (a.i % b.i == 0) return INTVAL(a.i / b.i); return REALVAL((double)a.i / (double)b.i); }
    if (ai && bi && !strcmp(op, "mod"))  { if (b.i == 0) return FAILDESCR; long long m = a.i % b.i; if (m && ((m < 0) != (b.i < 0))) m += b.i; return INTVAL(m); }
    return rt_num_arith(a, b, pl_is_op_code(op));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_atom_str(DESCR_t v) {
    if (v.v == DT_S) return v.s ? v.s : "";
    if (v.v == (DTYPE_t)DT_A) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm ? nm : ""; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_mk_atom(const char *s) { DESCR_t d; d.v = DT_S; d.slen = (uint32_t)strlen(s); d.s = s; return d; }
static DESCR_t pl_nil(void) { return pl_mk_atom("[]"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_cons(DESCR_t head, DESCR_t tail) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_plj_alloc(2 * sizeof(DESCR_t)); kids[0] = head; kids[1] = tail;
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(".")) << 16) | (2u & 0xFFFFu); c.p = (void *)kids;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_list_from_arr(DESCR_t *elems, int n) { DESCR_t acc = pl_nil(); for (int i = n - 1; i >= 0; i--) acc = pl_cons(elems[i], acc); return acc; }
static void *pl_var_cell_ptr(DESCR_t v) { extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t d = rt_pl_deref_val(v); return (d.v == (DTYPE_t)DT_PLVAR) ? d.p : (void *)0; }
static void pl_count_var_occ(DESCR_t t, void *target, int *cnt) { extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t d = rt_pl_deref_val(t); if (d.v == (DTYPE_t)DT_PLVAR) { if (d.p == target) (*cnt)++; return; } if (d.v == (DTYPE_t)DT_PLREF) { int ar = (int)(d.slen & 0xFFFFu); DESCR_t *kids = (DESCR_t *)d.p; for (int i = 0; i < ar; i++) pl_count_var_occ(kids[i], target, cnt); } }
static DESCR_t pl_mk_atom_dup(const char *s, size_t n) { extern int prolog_atom_intern(const char *); char *o = (char *)rt_ws_alloc(n + 1); if (n) memcpy(o, s, n); o[n] = 0; DESCR_t d; d.v = DT_S; d.slen = (uint32_t)n; d.s = o; (void)prolog_atom_intern(o); return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sink_kind(DESCR_t a) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(a);
    if ((int)v.v != DT_PLREF) return 0;
    int fid = (int)(v.slen >> 16); int ar = (int)(v.slen & 0xFFFFu); if (ar != 1) return 0;
    const char *fnm = prolog_atom_name(fid); if (!fnm) return 0;
    if (!strcmp(fnm, "atom")) return 1;
    if (!strcmp(fnm, "string")) return 2;
    if (!strcmp(fnm, "codes")) return 3;
    if (!strcmp(fnm, "chars")) return 4;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_sink_build(int kind, const char *s, size_t n) {
    if (kind == 1 || kind == 2) return pl_mk_atom_dup(s, n);
    DESCR_t *elems = (DESCR_t *)rt_ws_alloc((n > 0 ? n : 1) * sizeof(DESCR_t));
    for (size_t i = 0; i < n; i++) {
        if (kind == 3) { elems[i].v = (DTYPE_t)DT_I; elems[i].slen = 0; elems[i].i = (unsigned char)s[i]; }
        else { char *o = (char *)rt_ws_alloc(2); o[0] = s[i]; o[1] = 0; elems[i] = pl_mk_atom_dup(o, 1); }
    }
    return pl_list_from_arr(elems, (int)n);
}
typedef struct { char *buf; size_t sz; int saved_out; int idx; } PlWotFrame;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_nil(DESCR_t v) {
    if (v.v == DT_S || v.v == DT_SNUL) return v.s && !strcmp(v.s, "[]");
    if (v.v == (DTYPE_t)DT_A) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm && !strcmp(nm, "[]"); }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_list_to_arr(DESCR_t lst, DESCR_t *out, int max) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    int n = 0; DESCR_t cur = rt_pl_deref_val(lst);
    while (cur.v == (DTYPE_t)DT_PLREF && (int)(cur.slen & 0xFFFFu) == 2) {
        DESCR_t *kids = (DESCR_t *)cur.p; if (n >= max) return -1;
        out[n++] = rt_pl_deref_val(kids[0]); cur = rt_pl_deref_val(kids[1]);
    }
    return pl_is_nil(cur) ? n : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_list_to_cstr(DESCR_t arg, int codes) {
    DESCR_t elems[4096]; int n = pl_list_to_arr(arg, elems, 4096);
    if (n < 0) return (char *)0;
    char *o = (char *)malloc((size_t)n + 1);
    if (!o) return (char *)0;
    for (int i = 0; i < n; i++) {
        if (codes) { if (elems[i].v != DT_I) { free(o); return (char *)0; } o[i] = (char)elems[i].i; }
        else { const char *cs = pl_atom_str(elems[i]); if (!cs || !cs[0]) { free(o); return (char *)0; } o[i] = cs[0]; }
    }
    o[n] = 0; return o;
}
static void out_write_descr(FILE *dest, DESCR_t av, int use_gist);
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
    char nm[200]; int ar = 0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    if (sl) { int kl = (int)(sl - key); if (kl > 199) kl = 199; memcpy(nm, key, (size_t)kl); nm[kl] = 0; ar = atoi(sl + 1); }
    else { snprintf(nm, sizeof nm, "%s", key ? key : "?"); ar = 0; }
    rt_pl_iso_throw_pi("existence_error", "procedure", nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission(const char *op, const char *type, const char *nm, int ar) { pl_iso_uncaught("permission_error(%s, %s, %s/%d)", op ? op : "access", type ? type : "private_procedure", nm, ar); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission_t(const char *op, const char *type, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("permission_error(%s, %s, %s%.0d)", op ? op : "access", type ? type : "stream", "_", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_type(const char *type, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("type_error(%s, %s%.0s%.0d)", type, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_domain(const char *domain, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("domain_error(%s, %s%.0s%.0d)", domain, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_existence(const char *type, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("existence_error(%s, %s%.0s%.0d)", type, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_proc_defined_static(const char *name, long arity) {
    if (!name) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%ld", name, arity);
    int idx = rt_proc_index_of(key);
    return (idx >= 0 && !rt_proc_is_generator(key)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_iso_evaluable(DESCR_t v) {
    extern const char *prolog_atom_name(int);
    if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); return; }
    if ((int)v.v == DT_A) { rt_pl_iso_throw_pi("type_error", "evaluable", prolog_atom_name((int)v.i), 0); return; }
    if (v.v == DT_S) { rt_pl_iso_throw_pi("type_error", "evaluable", v.s, 0); return; }
    if ((int)v.v == DT_PLREF) { rt_pl_iso_throw_pi("type_error", "evaluable", prolog_atom_name((int)(v.slen >> 16)), (int)(v.slen & 0xFFFFu)); return; }
    rt_pl_iso_throw_pi("type_error", "evaluable", "?", 0);
}
static long g_tap_run = 0;
static long g_tap_failed = 0;
static long g_tap_planned = 0;
static int g_tap_no_plan = 1;
static long g_tap_todo_upto = 0;
static char g_tap_todo_reason[512] = "";
static int g_tap_done_run = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tap_truthy(DESCR_t v) {
    if (IS_FAIL_fn(v)) return 0;
    if (IS_INT_fn(v)) return (v.i != 0);
    if (IS_REAL_fn(v)) return (v.r != 0.0);
    if (v.v == DT_SNUL) return 0;
    { const char *s = v.s ? v.s : ""; return (s[0] != '\0' && !(s[0] == '0' && s[1] == '\0')); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tap_desc_escape(const char *desc, char *out, size_t cap) {
    size_t o = 0; if (!desc) { out[0] = '\0'; return; }
    for (size_t i = 0; desc[i] && o + 4 < cap; i++) {
        if (desc[i] == '#') { out[o++] = ' '; out[o++] = '\\'; out[o++] = '#'; }
        else if (desc[i] == '\n') { out[o++] = '\n'; out[o++] = '#'; out[o++] = ' '; }
        else out[o++] = desc[i];
    }
    out[o] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tap_proclaim(int cond, const char *desc, const char *prefix) {
    char esc[1024]; char line[2048]; int in_todo;
    g_tap_run++;
    in_todo = (g_tap_todo_reason[0] != '\0' && g_tap_run <= g_tap_todo_upto);
    if (!cond && !(g_tap_run <= g_tap_todo_upto)) g_tap_failed++;
    rk_tap_desc_escape(desc ? desc : "", esc, sizeof esc);
    snprintf(line, sizeof line, "%sok %ld - %s%s%s", cond ? "" : "not ", g_tap_run, prefix ? prefix : "", esc, in_todo ? g_tap_todo_reason : "");
    puts(line); fflush(stdout);
    return cond;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tap_diag(const char *msg) {
    fprintf(stderr, "# %s\n", msg ? msg : ""); fflush(stderr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen) {
    size_t cap = strlen(fmt) + 64, len = 0; char *buf = (char *)rt_ws_alloc(cap); int ai = from;
    for (const char *p = fmt; *p; ) {
        if (*p != '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = *p++; continue; }
        const char *start = p; p++;
        if (*p == '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = '%'; p++; continue; }
        char spec[64]; int sp = 0; spec[sp++] = '%';
        while (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#') { if (sp < 60) spec[sp++] = *p; p++; }
        long width = -1;
        if (*p == '*') { width = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0; ai++; sp += snprintf(spec + sp, sizeof spec - sp, "%ld", width); p++; }
        else while (*p >= '0' && *p <= '9') { if (sp < 60) spec[sp++] = *p; p++; }
        if (*p == '.') { if (sp < 60) spec[sp++] = '.'; p++;
            if (*p == '*') { long prec = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0; ai++; sp += snprintf(spec + sp, sizeof spec - sp, "%ld", prec); p++; }
            else while (*p >= '0' && *p <= '9') { if (sp < 60) spec[sp++] = *p; p++; } }
        char conv = *p; if (!conv) { if (len + 1 < cap) buf[len++] = '%'; break; } p++;
        DESCR_t a = (ai < nargs) ? args[ai] : NULVCL; ai++;
        char piece[512]; char sb[256]; int done = 0;
        if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'x' || conv == 'X' || conv == 'o') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 'l'; cspec[cl++] = conv; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, lv); done = 1;
        } else if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'F' || conv == 'g' || conv == 'G') {
            double dv = IS_REAL_fn(a) ? a.r : (IS_INT_fn(a) ? (double)a.i : atof(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = conv; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, dv); done = 1;
        } else if (conv == 'c') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 'c'; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, (int)lv); done = 1;
        } else if (conv == 's') {
            const char *sv = to_cstring(a, sb, sizeof sb); if (!sv) sv = "";
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 's'; cspec[cl] = 0;
            int need = snprintf(NULL, 0, cspec, sv);
            if (need >= (int)sizeof piece) { char *big = (char *)rt_ws_alloc((size_t)need + 1); snprintf(big, (size_t)need + 1, cspec, sv);
                if (len + (size_t)need + 1 > cap) { cap = len + (size_t)need + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; }
                memcpy(buf + len, big, (size_t)need); len += (size_t)need; continue; }
            snprintf(piece, sizeof piece, cspec, sv); done = 1;
        } else if (conv == 'b') {
            unsigned long uv = IS_INT_fn(a) ? (unsigned long)a.i : (IS_REAL_fn(a) ? (unsigned long)a.r : (unsigned long)atol(to_cstring(a, sb, sizeof sb)));
            char bits[72]; int bn = 0; if (uv == 0) bits[bn++] = '0'; else { char tmp[72]; int tn = 0; while (uv) { tmp[tn++] = (char)('0' + (uv & 1)); uv >>= 1; } while (tn) bits[bn++] = tmp[--tn]; } bits[bn] = 0;
            long w = 0; int zero = 0; for (int k = 1; k < sp; k++) { if (spec[k] == '0') zero = 1; else if (spec[k] >= '1' && spec[k] <= '9') { w = w * 10 + (spec[k] - '0'); } }
            int pad = (int)w - bn; if (pad > 0) { char pc = zero ? '0' : ' '; for (int k = 0; k < pad; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = pc; } }
            for (int k = 0; k < bn; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = bits[k]; } continue;
        } else { for (const char *q = start; q <= start + (p - start) - 1; q++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = *q; } continue; }
        if (done) { size_t pl = strlen(piece); if (len + pl + 1 > cap) { cap = len + pl + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } memcpy(buf + len, piece, pl); len += pl; }
    }
    buf[len] = 0; *outp = buf; if (outlen) *outlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_eval(DESCR_t t, DESCR_t *out, void **ball);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *pl_ax_int_ball(DESCR_t a, DESCR_t b, int ai) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    return rt_pl_ball_kind2("type_error", "integer", ai ? b : a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_ax(const char *op, DESCR_t *args, int nargs, DESCR_t *out, void **ball) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    if (nargs == 0) { if (!strcmp(op, "pi")) { *out = REALVAL(M_PI); return 1; } if (!strcmp(op, "e")) { *out = REALVAL(M_E); return 1; } *out = FAILDESCR; return 1; }
    DESCR_t a = rt_pl_deref_val(args[0]);
    if (a.v != DT_I && a.v != DT_R) { void *bl = (void *)0; DESCR_t ev; if (pl_ax_eval(a, &ev, &bl)) a = ev; else { if (bl && ball && !*ball) *ball = bl; *out = FAILDESCR; return 1; } }
    int ai = (a.v == DT_I), arl = (a.v == DT_R);
    double ad = arl ? a.r : (double)a.i;
    if (nargs == 1) {
        if (!strcmp(op, "neg"))   { *out = ai ? INTVAL(-a.i) : REALVAL(-ad); return 1; }
        if (!strcmp(op, "pos"))   { *out = a; return 1; }
        if (!strcmp(op, "abs"))   { *out = ai ? INTVAL(a.i < 0 ? -a.i : a.i) : REALVAL(fabs(ad)); return 1; }
        if (!strcmp(op, "sign"))  { *out = ai ? INTVAL((a.i > 0) - (a.i < 0)) : REALVAL((double)((ad > 0) - (ad < 0))); return 1; }
        if (!strcmp(op, "trunc")) { *out = INTVAL((long long)ad); return 1; }
        if (!strcmp(op, "intg"))  { *out = ai ? a : INTVAL((long long)llround(ad)); return 1; }
        if (!strcmp(op, "flt"))   { *out = REALVAL(ad); return 1; }
        if (!strcmp(op, "floor")) { *out = INTVAL((long long)floor(ad)); return 1; }
        if (!strcmp(op, "ceil"))  { *out = INTVAL((long long)ceil(ad)); return 1; }
        if (!strcmp(op, "round")) { *out = INTVAL((long long)llround(ad)); return 1; }
        if (!strcmp(op, "sqrt"))  { *out = REALVAL(sqrt(ad)); return 1; }
        if (!strcmp(op, "sin"))   { *out = REALVAL(sin(ad)); return 1; }
        if (!strcmp(op, "cos"))   { *out = REALVAL(cos(ad)); return 1; }
        if (!strcmp(op, "atan"))  { *out = REALVAL(atan(ad)); return 1; }
        if (!strcmp(op, "log"))   { *out = REALVAL(log(ad)); return 1; }
        if (!strcmp(op, "exp"))   { *out = REALVAL(exp(ad)); return 1; }
        if (!strcmp(op, "fip"))   { *out = REALVAL(trunc(ad)); return 1; }
        if (!strcmp(op, "ffp"))   { *out = REALVAL(ad - trunc(ad)); return 1; }
        if (!strcmp(op, "msb"))   { if (!ai) { if (ball && !*ball) *ball = pl_ax_int_ball(a, a, 0); *out = FAILDESCR; return 1; } if (a.i <= 0) { *out = FAILDESCR; return 1; } *out = INTVAL(63 - __builtin_clzll((unsigned long long)a.i)); return 1; }
        if (!strcmp(op, "bnot"))  { if (!ai) { extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t); if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "integer", a); *out = FAILDESCR; return 1; } *out = INTVAL(~a.i); return 1; }
        *out = FAILDESCR; return 1;
    }
    DESCR_t b = rt_pl_deref_val(args[1]);
    if (b.v != DT_I && b.v != DT_R) { void *bl = (void *)0; DESCR_t ev; if (pl_ax_eval(b, &ev, &bl)) b = ev; else { if (bl && ball && !*ball) *ball = bl; *out = FAILDESCR; return 1; } }
    int bi = (b.v == DT_I), brl = (b.v == DT_R);
    double bd = brl ? b.r : (double)b.i;
    if (!strcmp(op, "fpow") || !strcmp(op, "pow")) { if (ai && bi && b.i >= 0) { long long r = 1, bs = a.i, e = b.i; int ovf = 0; while (e) { if (e & 1) ovf |= __builtin_mul_overflow(r, bs, &r); e >>= 1; if (e) ovf |= __builtin_mul_overflow(bs, bs, &bs); } if (!ovf) { *out = INTVAL(r); return 1; } } *out = REALVAL(pow(ad, bd)); return 1; }
    if (!strcmp(op, "min")) { *out = (ai && bi) ? INTVAL(a.i < b.i ? a.i : b.i) : REALVAL(ad < bd ? ad : bd); return 1; }
    if (!strcmp(op, "max")) { *out = (ai && bi) ? INTVAL(a.i > b.i ? a.i : b.i) : REALVAL(ad > bd ? ad : bd); return 1; }
    if (!strcmp(op, "gcd")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } long long x = a.i < 0 ? -a.i : a.i, y = b.i < 0 ? -b.i : b.i; while (y) { long long t2 = x % y; x = y; y = t2; } *out = INTVAL(x); return 1; }
    if (!strcmp(op, "rem")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } if (b.i == 0) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i % b.i); return 1; }
    if (!strcmp(op, "xor")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i ^ b.i); return 1; }
    if (!strcmp(op, "shl")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i << b.i); return 1; }
    if (!strcmp(op, "shr")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i >> b.i); return 1; }
    if (!strcmp(op, "band")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i & b.i); return 1; }
    if (!strcmp(op, "bor"))  { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i | b.i); return 1; }
    { DESCR_t r = pl_arith2(op, a, b); if (r.v == DT_FAIL) { *out = FAILDESCR; return 1; } *out = r; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_eval(DESCR_t t, DESCR_t *out, void **ball) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern const char *prolog_atom_name(int);
    extern void *rt_pl_ball_instantiation(void); extern void *rt_pl_ball_type_pi(const char *, const char *, const char *, int);
    DESCR_t d = rt_pl_deref_val(t);
    if (d.v == DT_I || d.v == DT_R) { *out = d; return 1; }
    *out = d;
    if (d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL) { *ball = rt_pl_ball_instantiation(); return 0; }
    if ((int)d.v == DT_A || d.v == DT_S) { const char *nm = pl_atom_str(d); const char *sfx = pl_ax_suffix_of(nm, 0);
      if (sfx) { DESCR_t none = FAILDESCR; dop_ax(sfx, &none, 0, out, ball); return out->v != DT_FAIL; }
      *ball = rt_pl_ball_type_pi("type_error", "evaluable", nm ? nm : "?", 0); return 0; }
    if ((int)d.v == DT_PLREF) { int ar = (int)(d.slen & 0xFFFFu); const char *nm = prolog_atom_name((int)(d.slen >> 16)); const char *sfx = pl_ax_suffix_of(nm, ar); DESCR_t *kids = (DESCR_t *)d.p; DESCR_t ev[2];
      if (!sfx || ar < 1 || ar > 2 || !kids) { *ball = rt_pl_ball_type_pi("type_error", "evaluable", nm ? nm : "?", ar); return 0; }
      for (int k = 0; k < ar; k++) if (!pl_ax_eval(kids[k], &ev[k], ball)) { *out = ev[k]; return 0; }
      dop_ax(sfx, ev, ar, out, ball); return out->v != DT_FAIL; }
    *ball = rt_pl_ball_type_pi("type_error", "evaluable", "?", 0); return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_mkc_kids(DESCR_t *srcs, int ar, pl_tr_ctx_t *cx) {
    DESCR_t *kids = (DESCR_t *)rt_plj_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(DESCR_t));
    for (int i = 0; i < ar; i++) {
        DESCR_t t = srcs[i];
        DESCR_t *F = plw_cell_deref(plw_entry(&t));
        if (plw_unbound_tag(F)) {
            kids[i].v = (DTYPE_t)DT_PLVAR; kids[i].slen = 0; kids[i].p = (void *)&kids[i];
            DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)&kids[i];
            plw_bind(F, r, cx);
        } else kids[i] = *F;
    }
    return kids;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t plw_mkc_build(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    const char *fname = VARVAL_fn(args[0]); if (!fname) fname = "?";
    int ar = nargs - 1;
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = plw_mkc_kids(args + 1, ar, cx);
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(fname)) << 16) | ((uint32_t)ar & 0xFFFFu); c.p = (void *)kids;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_cmp(const char *op, DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t a = rt_pl_deref_val(args[0]); DESCR_t b = rt_pl_deref_val(args[1]);
    if (pl_num_cmp(op, a, b)) { *out = a; return 1; }
    *out = FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef int (*dop_body_fn)(DESCR_t *, int, DESCR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef int (*dop_axbody_fn)(DESCR_t *, int, DESCR_t *, void **);
static DESCR_t dop_call_ax(dop_axbody_fn body, DESCR_t *args, int nargs, void **ball) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    DESCR_t out = FAILDESCR;
    rt_gc_point_arr(args, nargs, (const char **)0);
    body(args, nargs, &out, ball);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_call(dop_body_fn body, DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    DESCR_t out = FAILDESCR;
    rt_gc_point_arr(args, nargs, (const char **)0);
    body(args, nargs, &out);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_ax_add(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("add", a, n, o, bl); }
static int dop_ax_sub(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("sub", a, n, o, bl); }
static int dop_ax_mul(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("mul", a, n, o, bl); }
static int dop_ax_div(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("div", a, n, o, bl); }
static int dop_ax_idiv(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("idiv", a, n, o, bl); }
static int dop_ax_mod(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax("mod", a, n, o, bl); }
static int dop_cmp_lt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("lt", a, n, o); }
static int dop_cmp_gt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("gt", a, n, o); }
static int dop_cmp_le(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("le", a, n, o); }
static int dop_cmp_ge(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ge", a, n, o); }
static int dop_cmp_eq(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("eq", a, n, o); }
static int dop_cmp_ne(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ne", a, n, o); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs != 2) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    rt_gc_point_arr(args, 2, (const char **)0);
    { char *tr0 = cx->tr; DESCR_t out;
      if (plw_unify_vals(args[0], args[1], cx)) out = rt_pl_deref_val(args[0]);
      else { cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); out = FAILDESCR; }
      rt_pl_tr_gc_sync(cx->tr); return out; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_ci_c(DESCR_t *args, long long imm, pl_tr_ctx_t *cx) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    DESCR_t out;
    rt_pl_tr_gc_sync(cx->tr);
    rt_gc_point_arr(args, 1, (const char **)0);
    { char *tr0 = cx->tr; DESCR_t t = args[0]; DESCR_t *c = plw_cell_deref(plw_entry(&t));
      if (plw_unbound_tag(c)) { DESCR_t w; w.v = DT_I; w.slen = 0; w.i = imm; plw_bind(c, w, cx); out = w; }
      else if (c->v == DT_I && !c->slen) out = (c->i == imm) ? *c : FAILDESCR;
      else if (c->v == (DTYPE_t)DT_PLREF) out = FAILDESCR;
      else { DESCR_t w; w.v = DT_I; w.slen = 0; w.i = imm; out = plw_unify_vals(args[0], w, cx) ? rt_pl_deref_val(args[0]) : FAILDESCR; }
      if (out.v == DT_FAIL) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); }
    rt_pl_tr_gc_sync(cx->tr);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_cs_c(DESCR_t *args, const char *cs, pl_tr_ctx_t *cx) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    DESCR_t out;
    rt_pl_tr_gc_sync(cx->tr);
    rt_gc_point_arr(args, 1, (const char **)0);
    { char *tr0 = cx->tr; DESCR_t t = args[0]; DESCR_t *c = plw_cell_deref(plw_entry(&t));
      if (plw_unbound_tag(c)) { DESCR_t w; w.v = DT_S; w.slen = cs ? (uint32_t)__builtin_strlen(cs) : 0u; w.s = cs; plw_bind(c, w, cx); out = w; }
      else if (c->v == (DTYPE_t)DT_PLREF) out = FAILDESCR;
      else { DESCR_t w; w.v = DT_S; w.slen = cs ? (uint32_t)__builtin_strlen(cs) : 0u; w.s = cs; out = plw_unify_vals(args[0], w, cx) ? rt_pl_deref_val(args[0]) : FAILDESCR; }
      if (out.v == DT_FAIL) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); }
    rt_pl_tr_gc_sync(cx->tr);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_mkc_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs < 1) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    rt_gc_point_arr(args, nargs, (const char **)0);
    { DESCR_t out = plw_mkc_build(args, nargs, cx); rt_pl_tr_gc_sync(cx->tr); return out; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_is_v_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs != 2) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    rt_gc_point_arr(args, 2, (const char **)0);
    { DESCR_t v = rt_pl_deref_val(args[1]); DESCR_t out;
      if (v.v != DT_I && v.v != DT_R) { void *bl = (void *)0; DESCR_t ev; if (pl_ax_eval(v, &ev, &bl)) v = ev; else { cx->ball = bl; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; } }
      if (v.v != DT_I && v.v != DT_R) { pl_iso_evaluable(v); out = FAILDESCR; }
      else { char *tr0 = cx->tr; out = plw_unify_vals(args[0], v, cx) ? v : FAILDESCR; if (out.v == DT_FAIL) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); }
      rt_pl_tr_gc_sync(cx->tr); return out; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_add_c(DESCR_t *args, int nargs, void **ball) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_add_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call_ax(dop_ax_add, args, nargs, ball) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_sub_c(DESCR_t *args, int nargs, void **ball) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_sub_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call_ax(dop_ax_sub, args, nargs, ball) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_mul_c(DESCR_t *args, int nargs, void **ball) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_mul_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call_ax(dop_ax_mul, args, nargs, ball) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_div_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_call_ax(dop_ax_div, args, nargs, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_ax_idiv_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_call_ax(dop_ax_idiv, args, nargs, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_ax_mod_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_call_ax(dop_ax_mod, args, nargs, ball) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_AX_LEAF(nm, op, ar) static int dop_ax_##nm(DESCR_t *a, int n, DESCR_t *o, void **bl) { return dop_ax(op, a, n, o, bl); } DESCR_t rt_pl_dop_ax_##nm##_c(DESCR_t *args, int nargs, void **ball) { return nargs == ar ? dop_call_ax(dop_ax_##nm, args, nargs, ball) : FAILDESCR; }
PL_AX_LEAF(rem, "rem", 2) PL_AX_LEAF(fpow, "fpow", 2) PL_AX_LEAF(pow, "pow", 2) PL_AX_LEAF(min, "min", 2) PL_AX_LEAF(max, "max", 2) PL_AX_LEAF(gcd, "gcd", 2) PL_AX_LEAF(xor, "xor", 2)
PL_AX_LEAF(shr, "shr", 2) PL_AX_LEAF(shl, "shl", 2) PL_AX_LEAF(band, "band", 2) PL_AX_LEAF(bor, "bor", 2)
PL_AX_LEAF(neg, "neg", 1) PL_AX_LEAF(pos, "pos", 1) PL_AX_LEAF(abs, "abs", 1) PL_AX_LEAF(sign, "sign", 1) PL_AX_LEAF(trunc, "trunc", 1) PL_AX_LEAF(intg, "intg", 1) PL_AX_LEAF(flt, "flt", 1)
PL_AX_LEAF(floor, "floor", 1) PL_AX_LEAF(ceil, "ceil", 1) PL_AX_LEAF(round, "round", 1) PL_AX_LEAF(sqrt, "sqrt", 1) PL_AX_LEAF(msb, "msb", 1) PL_AX_LEAF(bnot, "bnot", 1) PL_AX_LEAF(sin, "sin", 1)
PL_AX_LEAF(cos, "cos", 1) PL_AX_LEAF(atan, "atan", 1) PL_AX_LEAF(log, "log", 1) PL_AX_LEAF(exp, "exp", 1) PL_AX_LEAF(fip, "fip", 1) PL_AX_LEAF(ffp, "ffp", 1)
PL_AX_LEAF(pi, "pi", 0) PL_AX_LEAF(e, "e", 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_cmp_fast(DESCR_t *args, int rel, dop_body_fn slow, void **ball) {
    DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]);
    if ((a.v == DT_I || a.v == DT_R) && (b.v == DT_I || b.v == DT_R)) {
        double av = IS_REAL_fn(a) ? a.r : (double)a.i, bv = IS_REAL_fn(b) ? b.r : (double)b.i;
        int t = rel == 0 ? av < bv : rel == 1 ? av > bv : rel == 2 ? av <= bv : rel == 3 ? av >= bv : rel == 4 ? av == bv : av != bv;
        return t ? a : FAILDESCR; }
    { void *bl = (void *)0; DESCR_t ev[2];
      if (!pl_ax_eval(a, &ev[0], &bl) || !pl_ax_eval(b, &ev[1], &bl)) { if (ball) *ball = bl; return FAILDESCR; }
      return dop_call(slow, ev, 2); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_cmp_lt_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 0, dop_cmp_lt, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_gt_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 1, dop_cmp_gt, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_le_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 2, dop_cmp_le, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_ge_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 3, dop_cmp_ge, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_eq_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 4, dop_cmp_eq, ball) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_ne_c(DESCR_t *args, int nargs, void **ball) { return nargs == 2 ? dop_cmp_fast(args, 5, dop_cmp_ne, ball) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dop_write(DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    extern FILE *fh_cur_out_fp(void);
    if (nargs != 1) return FAILDESCR;
    rt_gc_point_arr(args, 1, (const char **)0);
    { FILE *wd = fh_cur_out_fp(); DESCR_t v = rt_pl_deref_val(args[0]);
      if (v.v == DT_R) { char fb[64]; fputs(pl_real_iso_str(v.r, fb, sizeof fb), wd); }
      else out_write_descr(wd, v, 0); }
    { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dop_nl(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    (void)args; (void)nargs;
    fputc('\n', fh_cur_out_fp());
    { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "../parsers/prolog/prolog_parse.h"
int rt_pl_keysort_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_compare_cell(void *, void *, void *, pl_tr_ctx_t *);int rt_pl_atop_cell(int, void *, void *);int rt_pl_acyclic_cell(void *);void rt_pl_writeq_cell(void *);
    void rt_pl_write_canonical_cell(void *);
void rt_pl_format_cell(const char *, void *);int rt_pl_char_type_cell(void *, void *, void *, pl_tr_ctx_t *);int rt_pl_term_string_cell(void *, void *, pl_tr_ctx_t *);
    int rt_pl_copy_term_cell(void *, void *, pl_tr_ctx_t *);
int rt_pl_term_variables_cell(void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_numbervars_cell(void *, void *, void *, pl_tr_ctx_t *); int rt_pl_numbervars1_cell(void *, pl_tr_ctx_t *); int rt_pl_sort_cell(int, void *, void *, pl_tr_ctx_t *);
static DESCR_t pl_ok(void) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
static void pl_atoms_ready(void) { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
static int pl_is_cons(DESCR_t d) { extern int ATOM_DOT; return d.v == (DTYPE_t)DT_PLREF && (int)(d.slen >> 16) == ATOM_DOT && (d.slen & 0xFFFFu) == 2; }
static int pl_val_unbound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL; }
static int pl_list_text(DESCR_t v, char *buf, size_t n) {
    size_t k = 0; DESCR_t d = rt_pl_deref_val(v);
    if (!pl_is_nil(d)) { const char *as = pl_atom_str(d); if (as) { size_t L = strlen(as); if (L + 1 > n) return 0; memcpy(buf, as, L + 1); return 1; } }
    while (pl_is_cons(d)) { DESCR_t *kids = (DESCR_t *)d.p; DESCR_t h = rt_pl_deref_val(kids[0]);
        if (h.v == DT_I) { if (k + 1 >= n) return 0; buf[k++] = (char)h.i; }
        else { const char *hs = pl_atom_str(h); if (!hs || strlen(hs) != 1 || k + 1 >= n) return 0; buf[k++] = hs[0]; }
        d = rt_pl_deref_val(kids[1]); }
    if (!pl_is_nil(d)) return 0;
    buf[k] = 0; return 1;
}
static int pl_cell_text(DESCR_t v, char *buf, size_t n, const char **out) {
    DESCR_t d = rt_pl_deref_val(v); const char *s = pl_atom_str(d);
    if (s) { *out = s; return 1; }
    if (d.v == DT_I) { snprintf(buf, n, "%lld", (long long)d.i); *out = buf; return 1; }
    if (d.v == DT_R) { *out = pl_real_iso_str(d.r, buf, (int)n); return 1; }
    if (pl_is_cons(d) || pl_is_nil(d)) { if (!pl_list_text(d, buf, n)) return 0; *out = buf; return 1; }
    return 0;
}
static DESCR_t pl_text_list(const char *s, int codes) { DESCR_t el[4096];int n = 0;for (;*s && n < 4096;s++) { if (codes) el[n++] = INTVAL((long long)(unsigned char)*s);
    else { char c2[2] = { *s, 0 };el[n++] = pl_mk_atom_dup(c2, 1);} } return pl_list_from_arr(el, n); }
static int pl_parse_number(const char *s, DESCR_t *out) { char *e = 0;long long iv = strtoll(s, &e, 10);if (e && e != s && !*e) { *out = INTVAL(iv);return 1;} double dv = strtod(s, &e);
    if (e && e != s && !*e) { *out = REALVAL(dv);return 1;} return 0; }
#define PL_TYPE_LEAF(nm) DESCR_t dop_pl_##nm(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 1 && rt_pl_type_test_cell(&args[0], #nm)) ? pl_ok() : FAILDESCR; }
PL_TYPE_LEAF(var) PL_TYPE_LEAF(nonvar) PL_TYPE_LEAF(atom) PL_TYPE_LEAF(number) PL_TYPE_LEAF(integer) PL_TYPE_LEAF(float)
PL_TYPE_LEAF(atomic) PL_TYPE_LEAF(compound) PL_TYPE_LEAF(callable) PL_TYPE_LEAF(ground) PL_TYPE_LEAF(is_list)
DESCR_t dop_pl_acyclic_term(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 1 && rt_pl_acyclic_cell(&args[0])) ? pl_ok() : FAILDESCR; }
#define PL_ATOP_LEAF(nm, op) DESCR_t dop_pl_atop_##nm(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 2 && rt_pl_atop_cell(op, &args[0], &args[1])) ? pl_ok() : FAILDESCR; }
PL_ATOP_LEAF(lt, 0) PL_ATOP_LEAF(le, 1) PL_ATOP_LEAF(gt, 2) PL_ATOP_LEAF(ge, 3) PL_ATOP_LEAF(eq, 4) PL_ATOP_LEAF(ne, 5)
DESCR_t dop_pl_writeq(DESCR_t *args, int nargs) { extern void rt_gc_point_arr(DESCR_t *, int, const char **);if (nargs != 1) return FAILDESCR;rt_gc_point_arr(args, 1, (const char **)0);
    rt_pl_writeq_cell(&args[0]);return pl_ok(); }
DESCR_t dop_pl_write_canonical(DESCR_t *args, int nargs) { extern void rt_gc_point_arr(DESCR_t *, int, const char **);if (nargs != 1) return FAILDESCR;rt_gc_point_arr(args, 1, (const char **)0);
    rt_pl_write_canonical_cell(&args[0]);return pl_ok(); }
DESCR_t dop_pl_writeln(DESCR_t *args, int nargs) { extern FILE *fh_cur_out_fp(void); if (nargs != 1) return FAILDESCR; dop_write(args, 1); fputc('\n', fh_cur_out_fp()); return pl_ok(); }
DESCR_t dop_pl_tab(DESCR_t *args, int nargs) { extern FILE *fh_cur_out_fp(void);if (nargs != 1) return FAILDESCR;{ DESCR_t v = rt_pl_deref_val(args[0]);if (v.v != DT_I) return FAILDESCR;
    FILE *o = fh_cur_out_fp();for (long long i = 0;i < v.i;i++) fputc(' ', o);} return pl_ok(); }
DESCR_t dop_pl_put_char(DESCR_t *args, int nargs) { extern FILE *fh_cur_out_fp(void);char b[64];const char *s;if (nargs != 1 || !pl_cell_text(args[0], b, sizeof b, &s)) return FAILDESCR;
    fputs(s, fh_cur_out_fp());return pl_ok(); }
DESCR_t dop_pl_halt(DESCR_t *args, int nargs) { int code = 0; if (nargs == 1) { DESCR_t v = rt_pl_deref_val(args[0]); if (v.v == DT_I) code = (int)v.i; } fflush((FILE *)0); exit(code); }
DESCR_t dop_pl_flush_output(DESCR_t *args, int nargs) { extern FILE *fh_cur_out_fp(void); (void)args; (void)nargs; fflush(fh_cur_out_fp()); return pl_ok(); }
DESCR_t dop_pl_format(DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *, int, const char **);
    char fb[4096]; const char *fmt; DESCR_t lst;
    if (nargs < 1 || nargs > 2) return FAILDESCR;
    pl_atoms_ready(); rt_gc_point_arr(args, nargs, (const char **)0);
    if (!pl_cell_text(args[0], fb, sizeof fb, &fmt)) return FAILDESCR;
    if (nargs == 2) { DESCR_t d = rt_pl_deref_val(args[1]); lst = (pl_is_cons(d) || pl_is_nil(d)) ? args[1] : pl_cons(args[1], pl_nil()); } else lst = pl_nil();
    rt_pl_format_cell(fmt, &lst);
    return pl_ok();
}
static int pl_stream_idx(DESCR_t s, int out) {
    extern FILE *fh_get(int); extern int prolog_atom_intern(const char *);
    DESCR_t d = rt_pl_deref_val(s); const char *nm = pl_atom_str(d);
    if (nm) { if (!strcmp(nm, "user_input")) return out ? -1 : 0; if (!strcmp(nm, "user_output")) return out ? 1 : -1;
        if (!strcmp(nm, "user_error")) return out ? 2 : -1; return -1; }
    if (d.v == (DTYPE_t)DT_PLREF && (int)(d.slen >> 16) == prolog_atom_intern("$stream") && (d.slen & 0xFFFFu) == 1) {
        DESCR_t a = rt_pl_deref_val(((DESCR_t *)d.p)[0]); if (a.v == DT_I && fh_get((int)a.i)) return (int)a.i; }
    return -1;
}
static DESCR_t pl_mk_stream(int idx) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); kids[0] = INTVAL((long long)idx);
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern("$stream")) << 16) | 1u; c.p = (void *)kids; return c;
}
#define PL_OUT_LEAF(nm, body1) DESCR_t dop_pl_##nm##_s(DESCR_t *args, int nargs) { extern int fh_current_output(void); extern void fh_set_output(int); \
    int idx = pl_stream_idx(args[0], 1); if (nargs < 1 || idx < 0) return FAILDESCR; \
    { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = body1(args + 1, nargs - 1); fh_set_output(sv); return r; } }
PL_OUT_LEAF(write, dop_write)
PL_OUT_LEAF(writeq, dop_pl_writeq)
PL_OUT_LEAF(write_canonical, dop_pl_write_canonical)
PL_OUT_LEAF(writeln, dop_pl_writeln)
PL_OUT_LEAF(nl, dop_nl)
PL_OUT_LEAF(tab, dop_pl_tab)
PL_OUT_LEAF(put_char, dop_pl_put_char)
PL_OUT_LEAF(flush_output, dop_pl_flush_output)
DESCR_t dop_pl_set_output(DESCR_t *args, int nargs) { extern void fh_set_output(int); int idx;
    if (nargs != 1 || (idx = pl_stream_idx(args[0], 1)) < 0) return FAILDESCR; fh_set_output(idx); return pl_ok(); }
DESCR_t dop_pl_set_input(DESCR_t *args, int nargs) { extern void fh_set_input(int); int idx;
    if (nargs != 1 || (idx = pl_stream_idx(args[0], 0)) < 0) return FAILDESCR; fh_set_input(idx); return pl_ok(); }
DESCR_t dop_pl_close(DESCR_t *args, int nargs) {
    extern FILE *fh_get(int); extern void fh_free(int); extern int fh_current_output(void); extern int fh_current_input(void);
    extern void fh_set_output(int); extern void fh_set_input(int);
    int idx; if (nargs < 1 || nargs > 2) return FAILDESCR; idx = pl_stream_idx(args[0], 1); if (idx < 0) idx = pl_stream_idx(args[0], 0); if (idx < 0) return FAILDESCR;
    if (idx >= 3) { FILE *fp = fh_get(idx); if (fp) fclose(fp); fh_free(idx);
        if (fh_current_output() == idx) fh_set_output(1); if (fh_current_input() == idx) fh_set_input(0); }
    return pl_ok();
}
DESCR_t dop_pl_op(DESCR_t *args, int nargs) {
    extern int prolog_op_table_add(const char *, int, const char *); char tb[64], nb[256]; const char *ty, *nm; DESCR_t p, n;
    if (nargs != 3) return FAILDESCR; p = rt_pl_deref_val(args[0]); if (p.v != DT_I || !pl_cell_text(args[1], tb, sizeof tb, &ty)) return FAILDESCR;
    ty = strdup(ty); n = rt_pl_deref_val(args[2]);
    if (pl_is_cons(n)) { while (pl_is_cons(n)) { DESCR_t *k = (DESCR_t *)n.p; if (!pl_cell_text(k[0], nb, sizeof nb, &nm) || !prolog_op_table_add(strdup(nm), (int)p.i, ty)) return FAILDESCR;
        n = rt_pl_deref_val(k[1]); } return pl_ok(); }
    if (!pl_cell_text(args[2], nb, sizeof nb, &nm) || !prolog_op_table_add(strdup(nm), (int)p.i, ty)) return FAILDESCR;
    return pl_ok();
}
static int pl_open_leaf(DESCR_t *args, pl_tr_ctx_t *cx) {
    extern int fh_alloc(FILE *); char fb[4096], mb[64]; const char *fn, *md; FILE *fp;
    if (!pl_cell_text(args[0], fb, sizeof fb, &fn) || !pl_cell_text(args[1], mb, sizeof mb, &md)) return 0;
    fp = fopen(fn, !strcmp(md, "read") ? "r" : !strcmp(md, "write") ? "w" : !strcmp(md, "append") ? "a" : !strcmp(md, "update") ? "r+" : "");
    if (!fp) return 0;
    { int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); return 0; } return plw_unify_vals(args[2], pl_mk_stream(idx), cx); }
}
#define PL_CX_LEAF_HEAD(nm, ar) DESCR_t rt_pl_dop_##nm##_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { extern void rt_gc_point_arr(DESCR_t *, int, const char **);int ok; \
    if (nargs != ar) return FAILDESCR;pl_atoms_ready();rt_pl_tr_gc_sync(cx->tr); rt_gc_point_arr(args, nargs, (const char **)0);
#define PL_CX_LEAF_TAIL rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
PL_CX_LEAF_HEAD(compare, 3) ok = rt_pl_compare_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(functor, 3) ok = rt_pl_functor_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(arg, 3) ok = rt_pl_arg_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(univ, 2) ok = rt_pl_univ_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(copy_term, 2) ok = rt_pl_copy_term_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(term_variables, 2) ok = rt_pl_term_variables_cell(&args[0], &args[1], (void *)0, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(numbervars3, 3) ok = rt_pl_numbervars_cell(&args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(numbervars1, 1) ok = rt_pl_numbervars1_cell(&args[0], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(succ, 2) ok = rt_pl_succ_plus_cell(2, &args[0], &args[1], (void *)0, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(plus, 3) ok = rt_pl_succ_plus_cell(3, &args[0], &args[1], &args[2], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(sort, 2) ok = rt_pl_sort_cell(0, &args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(msort, 2) ok = rt_pl_sort_cell(1, &args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(char_type, 2) ok = rt_pl_char_type_cell(&args[0], &args[1], (void *)0, cx); PL_CX_LEAF_TAIL
static long pl_sub_atom_count(DESCR_t a) {
    char sb[8192]; DESCR_t av = rt_pl_deref_val(a);
    if (av.v != DT_S) return -1;
    { const char *s = to_cstring(av, sb, sizeof sb); if (!s) return -1; return (long)strlen(s); }
}
DESCR_t rt_pl_dop_sub_atom_n(DESCR_t *args, int nargs) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    { long n = pl_sub_atom_count(args[0]);
      if (n < 0) return FAILDESCR;
      return INTVAL((n + 1) * (n + 2) / 2 - 1); }
}
static int rt_pl_sub_atom_at_cell(DESCR_t *args, pl_tr_ctx_t *cx) {
    char sb[8192]; DESCR_t av = rt_pl_deref_val(args[0]); DESCR_t iv = rt_pl_deref_val(args[1]);
    if (av.v != DT_S || iv.v != DT_I) return 0;
    { const char *s = to_cstring(av, sb, sizeof sb);
      long n = s ? (long)strlen(s) : -1, idx = (long)iv.i, b = 0, l, a;
      if (!s || n < 0 || idx < 0) return 0;
      while (b <= n) { long w = n - b + 1; if (idx < w) break; idx -= w; b++; }
      if (b > n) return 0;
      l = idx; a = n - b - l;
      { char *tr0 = cx->tr;
        int ok = plw_unify_vals(args[2], INTVAL(b), cx) && plw_unify_vals(args[3], INTVAL(l), cx)
              && plw_unify_vals(args[4], INTVAL(a), cx) && plw_unify_vals(args[5], pl_mk_atom_dup(s + b, (size_t)l), cx);
        if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
        return ok; } }
}
PL_CX_LEAF_HEAD(sub_atom_at, 6) ok = rt_pl_sub_atom_at_cell(args, cx); PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_findall_new(DESCR_t *args, int nargs) {
    (void)args;
    if (nargs != 0) return FAILDESCR;
    pl_atoms_ready();
    { extern void *rt_pl_findall_begin(void); void *a = rt_pl_findall_begin();
      if (!a) return FAILDESCR;
      return INTVAL((int64_t)(intptr_t)a); }
}
DESCR_t rt_pl_dop_findall_add(DESCR_t *args, int nargs) {
    if (nargs != 2) return FAILDESCR;
    { extern void rt_pl_findall_collect(void *, void *);
      DESCR_t h = rt_pl_deref_val(args[0]);
      if (h.v != DT_I) return FAILDESCR;
      rt_pl_findall_collect((void *)(intptr_t)h.i, (void *)&args[1]);
      return pl_ok(); }
}
static int rt_pl_all_solutions_cell(DESCR_t *args, pl_tr_ctx_t *cx, int mode) {
    extern int rt_pl_findall_count(void *);
    extern void rt_pl_findall_item(void *, int, void *);
    extern int rt_pl_sort_cell(int, void *, void *, pl_tr_ctx_t *);
    DESCR_t h = rt_pl_deref_val(args[0]);
    if (h.v != DT_I) return 0;
    { void *acc = (void *)(intptr_t)h.i; int n = rt_pl_findall_count(acc); int i;
      DESCR_t *el; DESCR_t lst;
      if (mode != 0 && n == 0) return 0;
      el = (DESCR_t *)rt_ws_alloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t));
      if (!el) return 0;
      for (i = 0; i < n; i++) rt_pl_findall_item(acc, i, (void *)&el[i]);
      lst = pl_list_from_arr(el, n);
      if (mode == 2) return rt_pl_sort_cell(0, (void *)&lst, (void *)&args[1], cx);
      return plw_unify_vals(args[1], lst, cx); }
}
PL_CX_LEAF_HEAD(findall_result, 2) ok = rt_pl_all_solutions_cell(args, cx, 0); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(bagof_result, 2)   ok = rt_pl_all_solutions_cell(args, cx, 1); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(setof_result, 2)   ok = rt_pl_all_solutions_cell(args, cx, 2); PL_CX_LEAF_TAIL
#define PL_ATOM_OP_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm, ar) ok = rt_pl_atom_op_cell(#nm, &args[0], ar > 1 ? (void *)&args[1] : (void *)0, ar > 2 ? (void *)&args[2] : (void *)0, cx); PL_CX_LEAF_TAIL
PL_ATOM_OP_LEAF(atom_length, 2) PL_ATOM_OP_LEAF(atom_concat, 3) PL_ATOM_OP_LEAF(atom_chars, 2) PL_ATOM_OP_LEAF(atom_codes, 2) PL_ATOM_OP_LEAF(atom_number, 2) PL_ATOM_OP_LEAF(atom_string, 2)
PL_ATOM_OP_LEAF(upcase_atom, 2) PL_ATOM_OP_LEAF(downcase_atom, 2) PL_ATOM_OP_LEAF(string_concat, 3) PL_ATOM_OP_LEAF(string_length, 2) PL_ATOM_OP_LEAF(string_lower, 2) PL_ATOM_OP_LEAF(string_upper, 2)
PL_ATOM_OP_LEAF(string_to_atom, 2) PL_ATOM_OP_LEAF(number_string, 2)
static int pl_split_text(DESCR_t *args, pl_tr_ctx_t *cx) {
    char sb[256], tb[8192]; const char *sep, *txt; DESCR_t el[4096]; int n = 0; size_t sl;
    if (!pl_cell_text(args[1], sb, sizeof sb, &sep) || !sep[0] || !pl_cell_text(args[2], tb, sizeof tb, &txt)) return 0;
    sl = strlen(sep);
    for (const char *p = txt;;) { const char *q = strstr(p, sep); size_t L = q ? (size_t)(q - p) : strlen(p); el[n++] = pl_mk_atom_dup(p, L); if (!q || n >= 4096) break; p = q + sl; }
    return plw_unify_vals(args[0], pl_list_from_arr(el, n), cx);
}
DESCR_t rt_pl_dop_atomic_list_concat_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { extern void rt_gc_point_arr(DESCR_t *, int, const char **); int ok;
    if (nargs != 2 && nargs != 3) return FAILDESCR;
    pl_atoms_ready(); rt_pl_tr_gc_sync(cx->tr); rt_gc_point_arr(args, nargs, (const char **)0);
    if (nargs == 3 && pl_val_unbound(rt_pl_deref_val(args[0]))) ok = pl_split_text(args, cx);
    else ok = rt_pl_atom_op_cell("atomic_list_concat", &args[0], &args[1], nargs == 3 ? (void *)&args[2] : (void *)0, cx); PL_CX_LEAF_TAIL
DESCR_t rt_pl_dop_concat_atom_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { extern void rt_gc_point_arr(DESCR_t *, int, const char **);int ok;if (nargs != 2 && nargs != 3) return FAILDESCR;
    pl_atoms_ready();rt_pl_tr_gc_sync(cx->tr); rt_gc_point_arr(args, nargs, (const char **)0);
    ok = rt_pl_atom_op_cell("concat_atom", &args[0], &args[1], nargs == 3 ? (void *)&args[2] : (void *)0, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(char_code, 2) { char b[64]; const char *s; DESCR_t a = rt_pl_deref_val(args[0]), c = rt_pl_deref_val(args[1]);
    if (pl_atom_str(a) && pl_cell_text(a, b, sizeof b, &s) && s[0]) ok = plw_unify_vals(args[1], INTVAL((long long)(unsigned char)s[0]), cx);
    else if (c.v == DT_I) { char c2[2] = { (char)c.i, 0 }; ok = plw_unify_vals(args[0], pl_mk_atom_dup(c2, 1), cx); } else ok = 0; } PL_CX_LEAF_TAIL
static int pl_number_text_leaf(DESCR_t *args, int codes, pl_tr_ctx_t *cx) { char b[4096]; const char *s; DESCR_t a = rt_pl_deref_val(args[0]);
    if (a.v == DT_I || a.v == DT_R) { pl_cell_text(a, b, sizeof b, &s); return plw_unify_vals(args[1], pl_text_list(s, codes), cx); }
    { DESCR_t num; if (!pl_list_text(args[1], b, sizeof b) || !pl_parse_number(b, &num)) return 0; return plw_unify_vals(args[0], num, cx); } }
PL_CX_LEAF_HEAD(number_codes, 2) ok = pl_number_text_leaf(args, 1, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(number_chars, 2) ok = pl_number_text_leaf(args, 0, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(name, 2) { char b[4096]; const char *s; DESCR_t a = rt_pl_deref_val(args[0]);
    if (a.v == DT_I || a.v == DT_R || pl_atom_str(a)) { pl_cell_text(a, b, sizeof b, &s); ok = plw_unify_vals(args[1], pl_text_list(s, 1), cx); }
    else { DESCR_t num; if (!pl_list_text(args[1], b, sizeof b)) ok = 0; else ok = plw_unify_vals(args[0], pl_parse_number(b, &num) ? num : pl_mk_atom_dup(b, strlen(b)), cx); } } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(get_char, 1) { extern FILE *fh_cur_in_fp(void); int c = fgetc(fh_cur_in_fp());
    if (c == EOF) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);else { char c2[2] = { (char)c, 0 };
    ok = plw_unify_vals(args[0], pl_mk_atom_dup(c2, 1), cx); } } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(peek_char, 1) { extern FILE *fh_cur_in_fp(void); int c = fgetc(fh_cur_in_fp());
    if (c == EOF) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);else { ungetc(c, fh_cur_in_fp());char c2[2] = { (char)c, 0 };
    ok = plw_unify_vals(args[0], pl_mk_atom_dup(c2, 1), cx); } } PL_CX_LEAF_TAIL
typedef struct { const char *nm[256]; DESCR_t v[256]; int cnt[256]; int n; } pl_vtab_t;
static DESCR_t pl_tree_cell(const tree_t *t, pl_vtab_t *vt) {
    extern int prolog_atom_intern(const char *); extern DESCR_t rt_pl_fresh_var_ref(void);
    if (!t) return pl_mk_atom("?");
    switch (t->t) {
    case TT_ILIT: return INTVAL((long long)t->v.ival);
    case TT_FLIT: return REALVAL(t->v.dval);
    case TT_QLIT: case TT_NAME: return pl_mk_atom_dup(t->v.sval ? t->v.sval : "?", strlen(t->v.sval ? t->v.sval : "?"));
    case TT_VAR: { const char *nm = t->v.sval; if (!nm || nm[0] == '_') return rt_pl_fresh_var_ref();
        for (int i = 0; i < vt->n; i++) if (!strcmp(vt->nm[i], nm)) { vt->cnt[i]++; return vt->v[i]; }
        if (vt->n >= 256) return rt_pl_fresh_var_ref();
        vt->nm[vt->n] = nm; vt->v[vt->n] = rt_pl_fresh_var_ref(); vt->cnt[vt->n] = 1; return vt->v[vt->n++]; }
    case TT_CUT: return pl_mk_atom("!");
    case TT_MAKELIST: { int bar = (t->v.ival == 1 && t->n > 0); DESCR_t acc = bar ? pl_tree_cell(t->c[t->n - 1], vt) : pl_nil();
        for (int i = (bar ? t->n - 2 : t->n - 1); i >= 0; i--) acc = pl_cons(pl_tree_cell(t->c[i], vt), acc); return acc; }
    case TT_FNC: case TT_UNIFY: case TT_IF: {
        const char *nm = t->t == TT_UNIFY ? "=" : t->t == TT_IF ? "->" : (t->v.sval ? t->v.sval : "?");
        if (t->n == 0) return pl_mk_atom_dup(nm, strlen(nm));
        { DESCR_t *kids = (DESCR_t *)rt_plj_alloc((size_t)t->n * sizeof(DESCR_t)); for (int i = 0; i < t->n; i++) kids[i] = pl_tree_cell(t->c[i], vt);
          DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(nm)) << 16) | ((uint32_t)t->n & 0xFFFFu); c.p = (void *)kids; return c; } }
    default: return pl_mk_atom("?");
    }
}
static int pl_read_term_text(char *buf, size_t n) {
    extern FILE *fh_cur_in_fp(void); FILE *in = fh_cur_in_fp();
    size_t k = 0; int c, q = 0, seen = 0;
    while ((c = fgetc(in)) != EOF) {
        if (!seen && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) continue;
        if (!seen && c == '%') { while ((c = fgetc(in)) != EOF && c != '\n') ; continue; }
        seen = 1; if (k + 2 >= n) return -1; buf[k++] = (char)c;
        if (q) { if (c == q) q = 0; continue; }
        if (c == '\'' || c == '"') { q = c; continue; }
        if (c == '.') { int d = fgetc(in); if (d != EOF) ungetc(d, in); if (d == EOF || d == ' ' || d == '\t' || d == '\n' || d == '\r' || d == '%') { buf[k] = 0; return 1; } }
    }
    buf[k] = 0; return seen ? 1 : 0;
}
static int pl_parse_term_text(const char *txt, DESCR_t *out, pl_vtab_t *vt, PlProgram **pg_out) {
    size_t L = strlen(txt); char *text = (char *)rt_ws_alloc(L + 16); size_t e;
    memcpy(text, "'$rd'(", 6); memcpy(text + 6, txt, L); e = 6 + L;
    while (e > 6 && (text[e - 1] == ' ' || text[e - 1] == '\n' || text[e - 1] == '\t' || text[e - 1] == '\r')) e--;
    if (e > 6 && text[e - 1] == '.') e--;
    text[e] = ')'; text[e + 1] = '.'; text[e + 2] = '\n'; text[e + 3] = 0;
    { PlProgram *pg = prolog_parse(text, "user_input"); const tree_t *tr = (pg && pg->head) ? pg->head->tr : (const tree_t *)0;
      if (tr && tr->t == TT_CLAUSE && tr->n >= 1) tr = tr->c[0];
      if (pg_out) *pg_out = pg;
      if (!pg || pg->nerrors || !tr || tr->t != TT_FNC || tr->n != 1) return 0;
      vt->n = 0; *out = pl_tree_cell(tr->c[0], vt); return 1; }
}
PL_CX_LEAF_HEAD(read, 1) { static char text[65536]; int got = pl_read_term_text(text, sizeof text - 8); DESCR_t t; pl_vtab_t vt;
    if (got <= 0) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);
    else ok = pl_parse_term_text(text, &t, &vt, (PlProgram **)0) && plw_unify_vals(args[0], t, cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(atom_to_term, 3) { char b[65536]; const char *txt; DESCR_t t; pl_vtab_t vt;
    if (pl_val_unbound(rt_pl_deref_val(args[0]))) ok = rt_pl_term_string_cell(&args[1], &args[0], cx) && plw_unify_vals(args[2], pl_nil(), cx);
    else if (!pl_cell_text(args[0], b, sizeof b, &txt) || !pl_parse_term_text(txt, &t, &vt, (PlProgram **)0)) ok = 0;
    else { DESCR_t el[256]; int n = 0; extern int prolog_atom_intern(const char *);
        for (int i = 0; i < vt.n; i++) { DESCR_t *kids = (DESCR_t *)rt_plj_alloc(2 * sizeof(DESCR_t)); kids[0] = pl_mk_atom_dup(vt.nm[i], strlen(vt.nm[i])); kids[1] = vt.v[i];
            { DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern("=")) << 16) | 2u; c.p = (void *)kids; el[n++] = c; } }
        ok = plw_unify_vals(args[1], t, cx) && plw_unify_vals(args[2], pl_list_from_arr(el, n), cx); } } PL_CX_LEAF_TAIL
static int pl_read_term_options_cell(DESCR_t opts, pl_vtab_t *vt, pl_tr_ctx_t *cx) { extern int prolog_atom_intern(const char *);
    DESCR_t o = rt_pl_deref_val(opts);
    while (pl_is_cons(o)) { DESCR_t *kids = (DESCR_t *)o.p; DESCR_t opt = rt_pl_deref_val(kids[0]);
        if (opt.v == (DTYPE_t)DT_PLREF && (opt.slen & 0xFFFFu) == 1) { DESCR_t *oa = (DESCR_t *)opt.p; int fid = (int)(opt.slen >> 16);
            int is_vars = fid == prolog_atom_intern("variables"), is_vn = fid == prolog_atom_intern("variable_names"), is_sing = fid == prolog_atom_intern("singletons");
            if (is_vars || is_vn || is_sing) { DESCR_t el[256]; int n = 0;
                for (int i = 0; i < vt->n; i++) { if (is_sing && vt->cnt[i] != 1) continue;
                    if (is_vars) { el[n++] = vt->v[i]; continue; }
                    { DESCR_t *k2 = (DESCR_t *)rt_plj_alloc(2 * sizeof(DESCR_t)); k2[0] = pl_mk_atom_dup(vt->nm[i], strlen(vt->nm[i])); k2[1] = vt->v[i];
                      DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern("=")) << 16) | 2u; c.p = (void *)k2; el[n++] = c; } }
                if (!plw_unify_vals(oa[0], pl_list_from_arr(el, n), cx)) return 0; } }
        o = rt_pl_deref_val(kids[1]); }
    return 1; }
#define PL_READ_TERM_LEAF(nm) PL_CX_LEAF_HEAD(nm, 3) { char b[65536]; DESCR_t t; pl_vtab_t vt; \
    ok = pl_list_text(args[0], b, sizeof b) && pl_parse_term_text(b, &t, &vt, (PlProgram **)0) && plw_unify_vals(args[1], t, cx) && pl_read_term_options_cell(args[2], &vt, cx); } PL_CX_LEAF_TAIL
PL_READ_TERM_LEAF(read_term_from_atom) PL_READ_TERM_LEAF(read_term_from_chars) PL_READ_TERM_LEAF(read_term_from_codes)
#define PL_IN_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm##_s, ar) { extern int fh_current_input(void); extern void fh_set_input(int); int idx = pl_stream_idx(args[0], 0); ok = 0; \
    if (idx >= 0) { int sv = fh_current_input(); fh_set_input(idx); DESCR_t r = rt_pl_dop_##nm##_c(args + 1, ar - 1, cx); fh_set_input(sv); \
        ok = (r.v == (DTYPE_t)DT_I); } } PL_CX_LEAF_TAIL
PL_IN_LEAF(read, 2)
PL_IN_LEAF(get_char, 2)
PL_IN_LEAF(peek_char, 2)
PL_CX_LEAF_HEAD(current_output, 1) { extern int fh_current_output(void); ok = plw_unify_vals(args[0], pl_mk_stream(fh_current_output()), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(current_input, 1) { extern int fh_current_input(void); ok = plw_unify_vals(args[0], pl_mk_stream(fh_current_input()), cx); } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(open, 3) ok = pl_open_leaf(args, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(open4, 4) ok = pl_open_leaf(args, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(keysort, 2) ok = rt_pl_keysort_cell(&args[0], &args[1], cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(format3, 3) { extern int fh_current_output(void); extern void fh_set_output(int); extern int fh_capture_begin(char **, size_t *, int *);
    extern void fh_capture_end(int, int); extern int prolog_atom_intern(const char *);
    DESCR_t d = rt_pl_deref_val(args[0]); int idx = pl_stream_idx(args[0], 1); ok = 0;
    if (idx >= 0) { int sv = fh_current_output(); fh_set_output(idx); ok = (dop_pl_format(args + 1, 2).v == (DTYPE_t)DT_I); fh_set_output(sv); }
    else if (d.v == (DTYPE_t)DT_PLREF && (d.slen & 0xFFFFu) == 1) {
        int f = (int)(d.slen >> 16);
        int kind = (f == prolog_atom_intern("atom") || f == prolog_atom_intern("string")) ? 1 : f == prolog_atom_intern("codes") ? 2 : f == prolog_atom_intern("chars") ? 3 : 0;
        if (kind) { char *buf = 0; size_t sz = 0; int sv = -1; int cap = fh_capture_begin(&buf, &sz, &sv);
            if (cap >= 0) { DESCR_t r = dop_pl_format(args + 1, 2); fh_capture_end(cap, sv);
                if (r.v == (DTYPE_t)DT_I) ok =
                    plw_unify_vals(((DESCR_t *)d.p)[0], kind == 1 ? pl_mk_atom_dup(buf ? buf : "", sz) : pl_text_list(buf ? buf : "", kind == 2), cx);
                free(buf); } } } } PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(term_string, 2) { char b[65536]; const char *txt; DESCR_t t; pl_vtab_t vt;
    if (pl_val_unbound(rt_pl_deref_val(args[0])) && pl_cell_text(args[1], b, sizeof b, &txt)) ok = pl_parse_term_text(txt, &t, &vt, (PlProgram **)0) && plw_unify_vals(args[0], t, cx);
    else ok = rt_pl_term_string_cell(&args[0], &args[1], cx); } PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_read_src_from_fp(FILE *f, char *rb, int cap) {
    int rn = 0; int rc; int rql = 0; char rq = 0;
    for (;;) { rc = fgetc(f); if (rc == EOF) break; if (rql) { if (rc == '\n') rql = 0; continue; } if (rc == ' ' || rc == '\t' || rc == '\n' || rc == '\r') continue; if (rc == '%') { rql = 1; continue; } break; }
    if (rc == EOF) return -1;
    for (;;) { if (rq) { if (rn < cap - 1) rb[rn++] = (char)rc; if (rc == rq) rq = 0; rc = fgetc(f); if (rc == EOF) break; continue; } if (rc == '\'' || rc == '"' || rc == '`') { rq = (char)rc; if (rn < cap - 1) rb[rn++] = (char)rc; rc = fgetc(f); if (rc == EOF) break; continue; } if (rc == '.') { int nx = fgetc(f); if (nx == EOF || nx == ' ' || nx == '\t' || nx == '\n' || nx == '\r' || nx == '%') break; if (rn < cap - 1) rb[rn++] = '.'; rc = nx; continue; } if (rn < cap - 1) rb[rn++] = (char)rc; rc = fgetc(f); if (rc == EOF) break; }
    rb[rn] = 0; return rn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_nested_agg(DESCR_t *args, int nargs) {
    if (nargs <= 0 || !args) { char *e = rt_ws_alloc(1); e[0] = '\0'; return STRVAL(e); }
    size_t total = 0;
    for (int i = 0; i < nargs; i++) { char scratch[64]; const char *cs = to_cstring(args[i], scratch, sizeof scratch); total += strlen(cs) + 1; }
    char *buf = rt_ws_alloc(total + 1); size_t p = 0;
    for (int i = 0; i < nargs; i++) { char scratch[64]; const char *cs = to_cstring(args[i], scratch, sizeof scratch); size_t L = strlen(cs); if (p > 0) buf[p++] = SOH; memcpy(buf + p, cs, L); p += L; }
    buf[p] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs) {
    if (nargs <= 0 || !args) { char *e = rt_ws_alloc(1); e[0] = '\0'; return STRVAL(e); }
    const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
    size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
    int nel = 0, cap = nargs * 64;
    for (int i = 0; i < nargs; i++) {
        char scratch[64];
        const char *cs = to_cstring(args[i], scratch, sizeof scratch);
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
            if (!nx) break;
            seg = nx + 1;
        }
    }
    size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
    char *buf = rt_ws_alloc(total + 1); size_t p = 0;
    for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
    buf[p] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long pas_ord_of(DESCR_t v) { if (IS_INT_fn(v)) return (long)v.i; if (IS_STR_fn(v)) { const char *s = VARVAL_fn(v); return (s && s[0]) ? (long)(unsigned char)s[0] : -1L; } return -1L; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PAS_SET_BYTES 32
static void pas_set_bits(DESCR_t v, unsigned char out[PAS_SET_BYTES]) { memset(out, 0, PAS_SET_BYTES); if (IS_STR_fn(v) && v.slen == PAS_SET_BYTES && v.s) { memcpy(out, v.s, PAS_SET_BYTES); return; } if (IS_INT_fn(v)) { long iv = v.i; for (int b = 0; b < 64; b++) if ((iv >> b) & 1L) out[b / 8] |= (unsigned char)(1u << (b % 8)); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!strcmp(fn, "where") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { *out = FAILDESCR; return 1; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { *out = FAILDESCR; return 1; }
        long pos = ftell(fp);
        if (pos < 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(pos + 1);
        return 1;
    }
    if (!strcmp(fn, "seek") && (nargs == 1 || nargs == 2)) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { *out = FAILDESCR; return 1; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { *out = FAILDESCR; return 1; }
        long o = (nargs == 2 && IS_INT_fn(args[1])) ? (long)args[1].i : 1L;
        int rc = (o > 0) ? fseek(fp, o - 1, SEEK_SET) : fseek(fp, o, SEEK_END);
        if (rc != 0) { *out = FAILDESCR; return 1; }
        *out = args[0]; return 1;
    }
    if (!fn) return 0;
    extern int fh_capture_begin(char **, size_t *, int *); extern void fh_capture_end(int, int);
    if (!strcmp(fn, "__rk_undef")) { (void) args; (void) nargs; *out = NULVCL; return 1; }
    if (!strcmp(fn, "__rk_exit") && nargs == 1) {
        long code = IS_INT_fn(args[0]) ? args[0].i : 0;
        exit((int) code);
    }
    if (!strcmp(fn, "__rk_arr_at") && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = NULVCL; return 1; } *out = b->data[i - b->lo]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (idx < 0) { *out = NULVCL; return 1; }
        if (idx == 0 && !*cur) { *out = NULVCL; return 1; }
        if (idx >= 1 && !strchr(cur, SOH) && (size_t)idx <= strlen(cur)) { *out = INTVAL((long long)(unsigned char)cur[idx - 1]); return 1; }
        const char *seg = cur; long k = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { size_t elen = nx ? (size_t)(nx - seg) : strlen(seg); *out = elem_to_descr(seg, elen); return 1; }
            if (!nx) { *out = NULVCL; return 1; }
            seg = nx + 1; k++;
        }
    }
    if (!strncmp(fn, "__rk_test_", 10)) {
        const char *op = fn + 10; char sb1[512]; char sb2[512]; char msg[1024];
        if (!strcmp(op, "plan")) { long n = (nargs > 0 && IS_INT_fn(args[0])) ? (long)args[0].i : 0; g_tap_planned = n; g_tap_no_plan = 0; printf("1..%ld\n", n); fflush(stdout); *out = NULVCL; return 1; }
        if (!strcmp(op, "ok")) { int c = (nargs > 0) ? rk_tap_truthy(args[0]) : 0; const char *d = (nargs > 1) ? to_cstring(args[1], sb1, sizeof sb1) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "nok")) { int c = (nargs > 0) ? !rk_tap_truthy(args[0]) : 1; const char *d = (nargs > 1) ? to_cstring(args[1], sb1, sizeof sb1) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "is")) { const char *g = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; const char *e = (nargs > 1) ? to_cstring(args[1], sb2, sizeof sb2) : ""; int c = !strcmp(g, e); char gd[512]; char ed[512]; snprintf(gd, sizeof gd, "%s", g); snprintf(ed, sizeof ed, "%s", e); const char *d = (nargs > 2) ? to_cstring(args[2], msg, sizeof msg) : ""; rk_tap_proclaim(c, d, ""); if (!c) { char b[1024]; snprintf(b, sizeof b, "expected: '%s'", ed); rk_tap_diag(b); snprintf(b, sizeof b, "     got: '%s'", gd); rk_tap_diag(b); } *out = INTVAL(c); return 1; }
        if (!strcmp(op, "isnt")) { const char *g = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; const char *e = (nargs > 1) ? to_cstring(args[1], sb2, sizeof sb2) : ""; int c = strcmp(g, e) != 0; const char *d = (nargs > 2) ? to_cstring(args[2], msg, sizeof msg) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "pass")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_proclaim(1, d, ""); *out = INTVAL(1); return 1; }
        if (!strcmp(op, "flunk")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_proclaim(0, d, ""); *out = INTVAL(0); return 1; }
        if (!strcmp(op, "diag")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_diag(d); *out = NULVCL; return 1; }
        if (!strcmp(op, "todo")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; long n = (nargs > 1 && IS_INT_fn(args[1])) ? (long)args[1].i : 1; g_tap_todo_upto = g_tap_run + n; snprintf(g_tap_todo_reason, sizeof g_tap_todo_reason, " # TODO %s", r); *out = NULVCL; return 1; }
        if (!strcmp(op, "skip")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; long n = (nargs > 1 && IS_INT_fn(args[1])) ? (long)args[1].i : 1; for (long i = 0; i < n; i++) rk_tap_proclaim(1, r, "# SKIP "); *out = NULVCL; return 1; }
        if (!strcmp(op, "skip_rest")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : "<unknown>"; if (g_tap_no_plan) { rk_tap_diag("A plan is required in order to use skip-rest"); *out = NULVCL; return 1; } for (long i = g_tap_run; i < g_tap_planned; ) { rk_tap_proclaim(1, r, "# SKIP "); i = g_tap_run; } *out = NULVCL; return 1; }
        if (!strcmp(op, "done")) { g_tap_done_run = 1; if (g_tap_no_plan) { g_tap_planned = g_tap_run; printf("1..%ld\n", g_tap_planned); fflush(stdout); } if ((g_tap_planned || g_tap_run) && g_tap_planned != g_tap_run) { snprintf(msg, sizeof msg, "You planned %ld test%s, but ran %ld", g_tap_planned, g_tap_planned == 1 ? "" : "s", g_tap_run); rk_tap_diag(msg); } if (g_tap_failed) { snprintf(msg, sizeof msg, "You failed %ld test%s of %ld", g_tap_failed, g_tap_failed == 1 ? "" : "s", g_tap_run); rk_tap_diag(msg); } *out = NULVCL; return 1; }
        *out = NULVCL; return 1;
    }
    if ((!strcmp(fn, "wall_us") || !strcmp(fn, "wall_ms")) && nargs == 0) {
        struct timespec _ts; clock_gettime(CLOCK_MONOTONIC, &_ts);
        int64_t us = (int64_t)_ts.tv_sec * 1000000 + (int64_t)_ts.tv_nsec / 1000;
        *out = INTVAL(!strcmp(fn, "wall_us") ? us : us / 1000); return 1;
    }
    if (!strcmp(fn, "__multi_call") && nargs >= 1) {
        const char *base = VARVAL_fn(args[0]); if (!base) { *out = FAILDESCR; return 1; }
        int na = nargs - 1; DESCR_t *aa = &args[1];
        char prefix[160]; int pl = snprintf(prefix, sizeof prefix, "%s$", base);
        static int acc_idx[256]; static char acc_names[256][160]; static char acc_types[256][8][32]; static int acc_var[256]; static int acc_fixed[256]; int nacc = 0;
        extern int rt_proc_enum_count(void); extern const char *rt_proc_enum_name(int i);
        int pcount = rt_proc_enum_count();
        for (int pi = 0; pi < pcount && nacc < 256; pi++) {
            const char *pn = rt_proc_enum_name(pi);
            if (!pn || strncmp(pn, prefix, (size_t)pl)) continue;
            const char *p = pn + pl; const char *e = strchr(p, '$');
            int arity = atoi(p);
            int nt = 0; const char *q = e ? e + 1 : (const char *)0;
            while (q && nt < 8) {
                const char *nx = strchr(q, '$'); int len = nx ? (int)(nx - q) : (int)strlen(q); if (len > 31) len = 31; memcpy(acc_types[nacc][nt], q, (size_t)len); acc_types[nacc][nt][len] = 0;
                nt++; if (!nx) break; q = nx + 1;
            }
            int isvar = (nt > 0 && !strcmp(acc_types[nacc][nt - 1], "Slurpy"));
            int fixed = isvar ? arity - 1 : arity;
            if (isvar ? (na < fixed) : (arity != na)) continue;
            int ok = 1; for (int i = 0; i < fixed && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], aa[i])) { ok = 0; break; }
            if (!ok) continue;
            acc_var[nacc] = isvar; acc_fixed[nacc] = fixed;
            acc_idx[nacc] = pi; snprintf(acc_names[nacc], sizeof acc_names[nacc], "%s", pn); nacc++;
        }
        if (nacc == 0) {
            extern void rt_script_die_surface(const char *msg); char m[256]; snprintf(m, sizeof m, "Cannot resolve caller %s(...); no candidate matches the argument types", base);
            rt_script_die_surface(m); *out = FAILDESCR; return 1;
        }
        int win = -1;
        for (int pass = 0; pass < 2 && win < 0; pass++)
        for (int i = 0; i < nacc; i++) { int beaten = 0; if (acc_var[i] != pass) continue;
            for (int j = 0; j < nacc; j++) { if (i == j || acc_var[j] != pass) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], acc_fixed[i])) { beaten = 1; break; } }
            if (!beaten) { win = i; break; } }
        if (win < 0) win = 0;
        const char *wname = acc_names[win]; (void)acc_idx;
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < na && k < 64; k++) g_call_args[k] = aa[k];
        *out = rt_call_proc_descr(wname, na); return 1;
    }
    if (!strcmp(fn, "__blk_ref") && nargs == 1) {
        const char *bn = VARVAL_fn(args[0]); DESCR_t b; b.v = DT_BLK; b.slen = 0; b.s = (char *)(bn ? bn : "");
        *out = b; return 1;
    }
    if (!strcmp(fn, "__blk_invoke") && nargs >= 1) {
        const char *bn = (args[0].v == DT_BLK) ? args[0].s : VARVAL_fn(args[0]); if (!bn || !*bn) { *out = FAILDESCR; return 1; }
        int na = nargs - 1; extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < na && k < 64; k++) g_call_args[k] = args[k + 1];
        *out = rt_call_proc_descr(bn, na); return 1;
    }
    if (!strcmp(fn, "__param_check") && nargs >= 2) {
        const char *ptype = VARVAL_fn(args[0]); if (!ptype) ptype = "Any";
        const char *pname = (nargs >= 3) ? VARVAL_fn(args[2]) : "?"; if (!pname) pname = "?";
        if (!rt_mc_accepts(ptype, args[1])) {
            extern void rt_script_die_surface(const char *msg);
            char m[256]; snprintf(m, sizeof m, "Type check failed in binding to parameter '$%s'; expected %s", pname, ptype);
            rt_script_die_surface(m); *out = FAILDESCR; return 1;
        }
        *out = args[1]; return 1;
    }
    if (!strcmp(fn, "__pas_sqr") && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d * d; *out = r; return 1; }
        long v = IS_INT_fn(args[0]) ? args[0].i : 0;
        *out = INTVAL(v * v); return 1;
    }
    if (!strcmp(fn, "__pas_alloc") && nargs == 0) {
        g_pas_heap_ctr++; DESCR_t *c = pas_heap_cell(g_pas_heap_ctr, 1);
        if (c) *c = INTVAL(0); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_alloc_rec") && nargs == 1) {
        long nf = IS_INT_fn(args[0]) ? args[0].i : 1; if (nf < 1) nf = 1;
        size_t len = (size_t)(nf * 2 - 1); char *seg = rt_ws_alloc(len + 1); size_t p = 0;
        for (long k = 0; k < nf; k++) { if (k) seg[p++] = SOH; seg[p++] = '0'; } seg[p] = '\0';
        g_pas_heap_ctr++; DESCR_t *c = pas_heap_cell(g_pas_heap_ctr, 1);
        if (c) *c = STRVAL(seg); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_dispose") && nargs == 1) {
        *out = NULVCL; return 1;
    }
    if (!strcmp(fn, "__pas_alpha_str") && nargs == 2) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        long lo = IS_INT_fn(args[1]) ? (long)args[1].i : 0; if (lo < 0) lo = 0;
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 2); size_t oi = 0; const char *pp = sa; long k = 0;
        while (*pp) { char *ep = NULL; long v = strtol(pp, &ep, 10); if (ep == pp) break; if (k >= lo) o[oi++] = (char)v; k++; pp = ep; if (*pp == SOH) pp++; else break; }
        o[oi] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_strcmp") && nargs == 2) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        const char *sb = VARVAL_fn(args[1]); if (!sb) sb = "";
        size_t la = (args[0].v == DT_S && args[0].slen != 0xFFFFFFFFu && sa == args[0].s) ? (size_t)args[0].slen : strlen(sa);
        size_t lb = (args[1].v == DT_S && args[1].slen != 0xFFFFFFFFu && sb == args[1].s) ? (size_t)args[1].slen : strlen(sb);
        size_t lm = la < lb ? la : lb;
        int c = lm ? memcmp(sa, sb, lm) : 0;
        if (c == 0) c = (la < lb) ? -1 : (la > lb) ? 1 : 0;
        *out = INTVAL(c < 0 ? -1 : c > 0 ? 1 : 0); return 1;
    }
    if (!strcmp(fn, "__pas_ca_pack") && nargs >= 1) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 1);
        for (size_t i = 0; i < sl; i++) o[i] = (sa[i] == SOH) ? '\x1e' : sa[i];
        o[sl] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_ca_unpack") && nargs >= 1) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 1);
        for (size_t i = 0; i < sl; i++) o[i] = (sa[i] == '\x1e') ? SOH : sa[i];
        o[sl] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_field_set") && nargs == 3) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[2]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[2]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        const char *s = cur; long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == idx) { tstart = s; tend = nx; break; } if (!nx) { tstart = NULL; break; } s = nx + 1; k++; }
        if (!tstart) { *out = args[2]; return 1; }
        size_t pre = (size_t)(tstart - cur); size_t post = tend ? strlen(tend) : 0; size_t rvl = strlen(rv);
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post); o[pre + rvl + post] = '\0';
        *hc = STRVAL(o); *out = args[2]; return 1;
    }
    if (!strcmp(fn, "__pas_nrec_pfield_set") && nargs == 3) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[2]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[2]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv0 = to_cstring(args[2], rb, sizeof rb); if (!rv0) rv0 = "";
        size_t rvl = strlen(rv0); char *rv = rt_ws_alloc(rvl + 1); for (size_t j = 0; j < rvl; j++) rv[j] = (rv0[j] == SOH) ? '\x05' : rv0[j]; rv[rvl] = '\0';
        const char *s = cur; long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == idx) { tstart = s; tend = nx; break; } if (!nx) { tstart = NULL; break; } s = nx + 1; k++; }
        if (!tstart) { *out = args[2]; return 1; }
        size_t pre = (size_t)(tstart - cur); size_t post = tend ? strlen(tend) : 0;
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post); o[pre + rvl + post] = '\0';
        *hc = STRVAL(o); *out = args[2]; return 1;
    }
    if (!strcmp(fn, "__pas_field_idx_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[3]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long fidx = IS_INT_fn(args[1]) ? args[1].i : 0; long eidx = IS_INT_fn(args[2]) ? args[2].i : 0; if (eidx < 1) { *out = args[3]; return 1; }
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fidx) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = args[3]; return 1; }
        size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
        unsigned char ch;
        if (IS_INT_fn(args[3])) {
            long cv = args[3].i; if (cv < 0) cv = 0; if (cv > 255) cv = 255; ch = (unsigned char)cv;
        } else { const char *vs = VARVAL_fn(args[3]); ch = (unsigned char)((vs && vs[0]) ? vs[0] : ' '); }
        size_t nflen = ((size_t)eidx > flen) ? (size_t)eidx : flen; char *nfb = rt_ws_alloc(nflen + 1);
        for (size_t j = 0; j < nflen; j++) nfb[j] = (j < flen) ? fstart[j] : ' '; nfb[eidx - 1] = (char)ch; nfb[nflen] = '\0';
        size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
        char *o = rt_ws_alloc(pre + nflen + post + 1); memcpy(o, cur, pre); memcpy(o + pre, nfb, nflen);
        if (fend) memcpy(o + pre + nflen, fend, post); o[pre + nflen + post] = '\0';
        *hc = STRVAL(o); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_deref") && nargs == 1) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        DESCR_t *hc = pas_heap_cell(n, 0);
        *out = hc ? *hc : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "__pas_deref_set") && nargs == 2) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        DESCR_t *hc = pas_heap_cell(n, 0); if (hc) *hc = args[1];
        *out = args[1]; return 1;
    }
    if (!strcmp(fn, "__pas_nrec_get") && nargs == 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        if (fi < 0 || ei < 0) { *out = INTVAL(0); return 1; }
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = INTVAL(0); return 1; }
        size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
        const char *p = fstart; const char *pend = fstart + flen; long ix = 0;
        for (;;) {
            const char *nx = (const char *)memchr(p, '\x05', (size_t)(pend - p)); const char *eend = nx ? nx : pend;
            if (ix == ei) { *out = elem_to_descr(p, (size_t)(eend - p)); return 1; } if (!nx) { *out = INTVAL(0); return 1; } p = nx + 1; ix++;
        }
    }
    if (!strcmp(fn, "__pas_nrec_update") && nargs == 4) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        *out = STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv)); return 1;
    }
    if (!strcmp(fn, "__pas_nrec_field_set") && nargs == 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; if (fi < 0) { *out = args[0]; return 1; }
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb); if (!rv) rv = "";
        size_t rvl = strlen(rv); char *nf = rt_ws_alloc(rvl + 1); for (size_t j = 0; j < rvl; j++) nf[j] = (rv[j] == SOH) ? '\x05' : rv[j]; nf[rvl] = '\0';
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = args[0]; return 1; }
        size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, nf, rvl); if (fend) memcpy(o + pre + rvl, fend, post); o[pre + rvl + post] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_nrec_deref_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[3]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        *hc = STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv)); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_fassign") && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = STRVAL(rt_ws_strdup_c(s)); return 1;
    }
    if (!strcmp(fn, "__pas_rewrite") && nargs == 1) {
        extern int fh_alloc(FILE *);
        if (IS_FH_fn(args[0])) {
            FILE *ofp = fh_get((int)args[0].i);
            if (ofp) { rewind(ofp); if (ftruncate(fileno(ofp), 0) == 0) { *out = args[0]; return 1; } }
        }
        const char *nm = VARVAL_fn(args[0]);
        FILE *fp;
        if (nm && nm[0]) {
            fp = fopen(nm, "w");
        } else {
            char tmpl[] = "/tmp/scrip_pas_XXXXXX";
            int fd = mkstemp(tmpl);
            fp = (fd >= 0) ? fdopen(fd, "w+") : (FILE *)0;
        }
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "__pas_reset") && nargs == 1) {
        extern int fh_alloc(FILE *);
        if (IS_FH_fn(args[0])) {
            FILE *ofp = fh_get((int)args[0].i);
            if (ofp) { rewind(ofp); *out = args[0]; return 1; }
        }
        const char *nm = VARVAL_fn(args[0]); if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "r"); if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "__pas_fclose") && nargs == 1) {
        extern FILE *fh_get(int); extern void fh_free(int);
        if (IS_FH_fn(args[0])) { int idx = (int)args[0].i; FILE *fp = fh_get(idx); if (fp && fp != stdout && fp != stderr && fp != stdin) { fflush(fp); fclose(fp); fh_free(idx); } }
        *out = NULVCL; return 1;
    }
    if (!strcmp(fn, "__pas_in") && nargs == 2) {
        long e = pas_ord_of(args[0]);
        unsigned char bits[PAS_SET_BYTES]; pas_set_bits(args[1], bits);
        int hit = (e >= 0 && e < PAS_SET_BYTES * 8) ? ((bits[e / 8] >> (e % 8)) & 1) : 0;
        *out = INTVAL(hit ? 1 : 0); return 1;
    }
    if (!strcmp(fn, "__pas_set")) {
        unsigned char *buf = (unsigned char *)rt_ws_alloc(PAS_SET_BYTES); memset(buf, 0, PAS_SET_BYTES);
        for (int k = 0; k < nargs; k++) { long e = pas_ord_of(args[k]); if (e >= 0 && e < PAS_SET_BYTES * 8) buf[e / 8] |= (unsigned char)(1u << (e % 8)); }
        *out = BSTRVAL((char *)buf, PAS_SET_BYTES); return 1;
    }
    if (nargs == 2 && (!strcmp(fn, "__pas_setuni") || !strcmp(fn, "__pas_setint") || !strcmp(fn, "__pas_setdif")
                    || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")
                    || !strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne"))) {
        unsigned char a[PAS_SET_BYTES], b[PAS_SET_BYTES]; pas_set_bits(args[0], a); pas_set_bits(args[1], b);
        if (!strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")) {
            const unsigned char *x = !strcmp(fn, "__pas_subset") ? a : b;
            const unsigned char *y = !strcmp(fn, "__pas_subset") ? b : a;
            int ok = 1; for (int i = 0; i < PAS_SET_BYTES; i++) if (x[i] & (unsigned char)~y[i]) { ok = 0; break; }
            *out = INTVAL(ok); return 1;
        }
        if (!strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne")) {
            int eq = memcmp(a, b, PAS_SET_BYTES) == 0;
            *out = INTVAL((!strcmp(fn, "__pas_seteq") ? eq : !eq) ? 1 : 0); return 1;
        }
        unsigned char *r = (unsigned char *)rt_ws_alloc(PAS_SET_BYTES);
        for (int i = 0; i < PAS_SET_BYTES; i++) {
            if      (!strcmp(fn, "__pas_setuni")) r[i] = a[i] | b[i];
            else if (!strcmp(fn, "__pas_setint")) r[i] = a[i] & b[i];
            else                                  r[i] = a[i] & (unsigned char)~b[i];
        }
        *out = BSTRVAL((char *)r, PAS_SET_BYTES); return 1;
    }
    if (nargs >= 1 && (!strcmp(fn, "__rk_jct_any") || !strcmp(fn, "__rk_jct_all")
                    || !strcmp(fn, "__rk_jct_one") || !strcmp(fn, "__rk_jct_none"))) {
        char flav = !strcmp(fn, "__rk_jct_any") ? 'a'
                  : !strcmp(fn, "__rk_jct_all") ? 'l'
                  : !strcmp(fn, "__rk_jct_one") ? 'o' : 'n';
        size_t total = 2;
        char **ms = rt_ws_alloc((size_t)nargs * sizeof(char *));
        size_t *mlen = rt_ws_alloc((size_t)nargs * sizeof(size_t));
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            size_t L = strlen(cs);
            char *cp = rt_ws_alloc(L + 1); memcpy(cp, cs, L + 1);
            ms[i] = cp; mlen[i] = L; total += 1 + L;
        }
        char *buf = rt_ws_alloc(total + 2);
        size_t p = 0; buf[p++] = '\x03'; buf[p++] = flav;
        for (int i = 0; i < nargs; i++) { buf[p++] = SOH; memcpy(buf + p, ms[i], mlen[i]); p += mlen[i]; }
        buf[p++] = '\x04'; buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "elems") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = INTVAL(0); return 1; }
        int cnt = 1; for (const char *p = cur; *p; p++) if (*p == SOH) cnt++;
        *out = INTVAL(cnt); return 1;
    }
    if ((!strcmp(fn, "__rk_arr") || !strcmp(fn, "__rk_arr_lit")) && nargs >= 0) {
        extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
        *out = rt_make_flat_agg(args, nargs); return 1;
    }
    if (!strcmp(fn, "__rk_arr_lit_item") && nargs >= 0) {
        extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
        DESCR_t inner = rt_make_flat_agg(args, nargs);
        const char *cur = VARVAL_fn(inner); if (!cur) cur = "";
        size_t n = strlen(cur); char *buf = rt_ws_alloc(n + 3); size_t p = 0;
        buf[p++] = '[';
        for (size_t i = 0; i < n; i++) buf[p++] = (cur[i] == SOH) ? ' ' : cur[i];
        buf[p++] = ']'; buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_materialize") && nargs == 1) {
        DESCR_t *a = 0; int n = 0;
        if (!rt_lv_is_list(args[0], &a, &n)) return 0;
        extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
        *out = rt_make_flat_agg(a, n); return 1;
    }
    if (!strcmp(fn, "__rk_pair") && nargs == 2) {
        *out = rt_make_nested_agg(args, 2); return 1;
    }
    if (!strcmp(fn, "__rk_named_call") && nargs >= 2) {
        extern const char *rt_proc_pname(const char *name, int k);
        char pb[256]; const char *pname = to_cstring(args[0], pb, sizeof pb);
        int npos = IS_INT_fn(args[1]) ? (int)args[1].i : 0;
        if (npos < 0) npos = 0;
        if (npos > RK_NAMED_MAX) npos = RK_NAMED_MAX;
        DESCR_t slots[RK_NAMED_MAX];
        for (int i = 0; i < RK_NAMED_MAX; i++) slots[i] = NULVCL;
        int np = 0; while (np < RK_NAMED_MAX && rt_proc_pname(pname, np)) np++;
        int maxslot = 0;
        extern int rt_proc_named_rest(const char *name);
        int nrest = rt_proc_named_rest(pname); int nrx = nrest > 0 ? nrest - 1 : -1;
        size_t hcap = 1; for (int i = 2 + npos; i + 1 < nargs; i += 2) { char kb0[128], vb0[256]; const char *k0 = to_cstring(args[i], kb0, sizeof kb0); const char *v0 = to_cstring(args[i + 1], vb0, sizeof vb0); hcap += (k0 ? strlen(k0) : 0) + (v0 ? strlen(v0) : 0) + 2; }
        char *hbuf = rt_ws_alloc(hcap); size_t hp = 0; hbuf[0] = '\0';
        for (int i = 0; i < npos && (2 + i) < nargs; i++) { slots[i] = args[2 + i]; if (i + 1 > maxslot) maxslot = i + 1; }
        for (int i = 2 + npos; i + 1 < nargs; i += 2) {
            char kb[128]; const char *k = to_cstring(args[i], kb, sizeof kb);
            int found = -1;
            for (int s = 0; s < np; s++) { if (s == nrx) continue; const char *pn = rt_proc_pname(pname, s); if (pn && k && !strcmp(pn, k)) { found = s; break; } }
            if (found < 0) {
                if (nrx < 0) continue;
                char vb2[256]; const char *v = to_cstring(args[i + 1], vb2, sizeof vb2); if (!k) k = ""; if (!v) v = "";
                if (hp > 0) hbuf[hp++] = SOH;
                size_t kl = strlen(k); memcpy(hbuf + hp, k, kl); hp += kl; hbuf[hp++] = '\x02';
                size_t vl = strlen(v); memcpy(hbuf + hp, v, vl); hp += vl; hbuf[hp] = '\0';
                continue;
            }
            slots[found] = args[i + 1]; if (found + 1 > maxslot) maxslot = found + 1;
        }
        if (nrx >= 0 && nrx < RK_NAMED_MAX) { slots[nrx] = STRVAL(hbuf); if (nrx + 1 > maxslot) maxslot = nrx + 1; }
        int total = np > maxslot ? np : maxslot;
        if (total > RK_NAMED_MAX) total = RK_NAMED_MAX;
        *out = invoke_method_proc(pname, slots, total); return 1;
    }
    if (!strcmp(fn, "__rk_rep") && nargs == 2) {
        char sb[256]; const char *src = to_cstring(args[0], sb, sizeof sb);
        long long n = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        if (!src) src = "";
        if (n < 0) n = 0;
        size_t L = strlen(src);
        char *buf = rt_ws_alloc(L * (size_t)(n > 0 ? n : 0) + 1); size_t p = 0;
        for (long long k = 0; k < n; k++) { memcpy(buf + p, src, L); p += L; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_range_arr") && nargs == 2) {
        long long lo = IS_INT_fn(args[0]) ? (long long)args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        long long hi = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        if (hi < lo) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        long long cnt = hi - lo + 1;
        char *buf = rt_ws_alloc((size_t)cnt * 24 + 1); size_t p = 0;
        for (long long v = lo; v <= hi; v++) { if (p > 0) buf[p++] = SOH; char eb[24]; int el = snprintf(eb, sizeof eb, "%lld", v); memcpy(buf + p, eb, (size_t)el); p += (size_t)el; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "rk_write") || !strcmp(fn, "rk_writes"))) {
        DESCR_t *tmp = (DESCR_t *)rt_ws_alloc((size_t)(nargs > 0 ? nargs : 1) * sizeof(DESCR_t));
        for (int _ri = 0; _ri < nargs; _ri++) {
            if (IS_REAL_fn(args[_ri])) { char *_rb = rt_ws_alloc(64); rk_real_str(args[_ri].r, _rb, 64); tmp[_ri] = STRVAL(_rb); }
            else tmp[_ri] = args[_ri];
        }
        *out = rt_call_arr(!strcmp(fn, "rk_write") ? "write" : "writes", tmp, nargs); return 1;
    }
    if (!strcmp(fn, "note") && !rt_proc_is_registered(fn)) {
        DESCR_t *tmp = (DESCR_t *)rt_ws_alloc((size_t)(nargs > 0 ? nargs : 1) * sizeof(DESCR_t));
        for (int _ri = 0; _ri < nargs; _ri++) {
            if (IS_REAL_fn(args[_ri])) { char *_rb = rt_ws_alloc(64); rk_real_str(args[_ri].r, _rb, 64); tmp[_ri] = STRVAL(_rb); }
            else tmp[_ri] = args[_ri];
        }
        for (int _ri = 0; _ri < nargs; _ri++) { char _sb[512]; const char *_cs = to_cstring(tmp[_ri], _sb, sizeof _sb); fputs(_cs ? _cs : "", stderr); }
        fputc('\n', stderr); fflush(stderr);
        *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "rk_write_arr") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        size_t n = strlen(cur); char *buf = rt_ws_alloc(n + 3); size_t p = 0;
        buf[p++] = '[';
        for (size_t i = 0; i < n; i++) buf[p++] = (cur[i] == SOH) ? ' ' : cur[i];
        buf[p++] = ']'; buf[p] = '\0';
        DESCR_t tmp1 = STRVAL(buf);
        *out = rt_call_arr("write", &tmp1, 1); return 1;
    }
    if (!strcmp(fn, "__rk_div") && nargs == 2) {
        extern void rt_script_die_surface(const char *msg);
        DESCR_t a = args[0], b = args[1];
        int ai = IS_INT_fn(a), arl = IS_REAL_fn(a), bi = IS_INT_fn(b), brl = IS_REAL_fn(b);
        double ad = arl ? a.r : (ai ? (double)a.i : 0.0), bd = brl ? b.r : (bi ? (double)b.i : 0.0);
        if (!ai && !arl) { char sa[64]; const char *cs = to_cstring(a, sa, sizeof sa); ad = cs ? strtod(cs, (char **)0) : 0.0; }
        if (!bi && !brl) { char sb[64]; const char *cs = to_cstring(b, sb, sizeof sb); bd = cs ? strtod(cs, (char **)0) : 0.0; }
        if (ai && bi) {
            if (b.i == 0) { rt_script_die_surface("Attempt to divide by zero"); *out = FAILDESCR; return 1; }
            if (b.i == -1) { *out = INTVAL(-a.i); return 1; }
            if ((a.i % b.i) == 0) { *out = INTVAL(a.i / b.i); return 1; }
            *out = REALVAL((double)a.i / (double)b.i); return 1;
        }
        if (bd == 0.0) { rt_script_die_surface("Attempt to divide by zero"); *out = FAILDESCR; return 1; }
        *out = REALVAL(ad / bd); return 1;
    }
    if (!strcmp(fn, "__rk_arr_slice") && nargs == 3) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        long long lo = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        long long hi = IS_INT_fn(args[2]) ? (long long)args[2].i : (IS_REAL_fn(args[2]) ? (long long)args[2].r : 0);
        if (lo < 0) lo = 0;
        if (hi < lo) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        char *buf = rt_ws_alloc(strlen(cs) + 1); size_t p = 0; const char *seg = cs; long long k = 0; int wrote = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (k >= lo && k <= hi) { if (wrote) buf[p++] = SOH; memcpy(buf + p, seg, L); p += L; wrote = 1; }
            if (!nx || k >= hi) break;
            seg = nx + 1; k++;
        }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_pick") && nargs >= 2) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        size_t tot = strlen(cs); int nsel = nargs - 1;
        const char **els = rt_ws_alloc((tot + 2) * sizeof(const char *)); size_t *lens = rt_ws_alloc((tot + 2) * sizeof(size_t));
        int nel = 0; const char *seg = cs;
        for (;;) { const char *nx = strchr(seg, SOH); els[nel] = seg; lens[nel] = nx ? (size_t)(nx - seg) : strlen(seg); nel++; if (!nx) break; seg = nx + 1; }
        char *buf = rt_ws_alloc((tot + 2) * (size_t)nsel + 2); size_t p = 0;
        for (int i = 1; i < nargs; i++) {
            long long k = IS_INT_fn(args[i]) ? (long long)args[i].i : (IS_REAL_fn(args[i]) ? (long long)args[i].r : 0);
            if (i > 1) buf[p++] = SOH;
            if (k >= 0 && k < nel) { memcpy(buf + p, els[k], lens[k]); p += lens[k]; }
        }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_xx") && nargs == 2) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        long cnt = IS_INT_fn(args[1]) ? args[1].i : (IS_REAL_fn(args[1]) ? (long)args[1].r : 0);
        if (cnt < 1) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        const char **els = rt_ws_alloc((size_t)64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)64 * sizeof(size_t));
        int nel = 0, cap = 64;
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
            if (!nx) break;
            seg = nx + 1;
        }
        size_t one = 0; for (int i = 0; i < nel; i++) one += lens[i] + 1;
        size_t total = one * (size_t)cnt; char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (long r = 0; r < cnt; r++) for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_hash")) {
        size_t total = 1; char kb[256]; char vb[256];
        for (int i = 0; i + 1 < nargs; i += 2) { total += strlen(to_cstring(args[i], kb, sizeof kb)) + strlen(to_cstring(args[i + 1], vb, sizeof vb)) + 2; }
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i + 1 < nargs; i += 2) { const char *k = to_cstring(args[i], kb, sizeof kb); const char *v = to_cstring(args[i + 1], vb, sizeof vb);
            if (p > 0) buf[p++] = '\x01'; size_t kl = strlen(k); memcpy(buf + p, k, kl); p += kl; buf[p++] = '\x02'; size_t vl = strlen(v); memcpy(buf + p, v, vl); p += vl; }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "reverse") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = nel - 1; i >= 0; i--) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "head") || !strcmp(fn, "tail")) && nargs >= 1) {
        int is_tail = (fn[0] == 't');
        long long n = 1; int listargs = nargs;
        if (nargs >= 2) {
            char nb[64]; const char *ns = to_cstring(args[nargs - 1], nb, sizeof nb);
            char *ep; long long v = strtoll(ns ? ns : "", &ep, 10);
            if (ns && ep > ns) n = v;
            listargs = nargs - 1;
        }
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < listargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (n < 0) n = 0;
        int lo, hi;
        if (is_tail) { lo = (int)(nel - n); if (lo < 0) lo = 0; hi = nel; }
        else         { lo = 0; hi = (int)(n < nel ? n : nel); }
        size_t total = 0; for (int i = lo; i < hi; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = lo; i < hi; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "unique") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                int dup = 0;
                for (int j = 0; j < nel; j++) if (lens[j] == L && memcmp(els[j], seg, L) == 0) { dup = 1; break; }
                if (!dup && nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_sort") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        for (int a = 1; a < nel; a++) {
            const char *keys = els[a]; size_t keyl = lens[a]; int b = a - 1;
            while (b >= 0) {
                const char *xs = els[b]; const char *ys = keys; char *xe; char *ye;
                long long xi = strtoll(xs, &xe, 10); long long yi = strtoll(ys, &ye, 10);
                int both_num = (*xe == '\0' && xe != xs && *ye == '\0' && ye != ys);
                int cmp = both_num ? (xi > yi ? 1 : xi < yi ? -1 : 0) : strcmp(xs, ys);
                if (cmp <= 0) break;
                els[b + 1] = els[b]; lens[b + 1] = lens[b]; b--;
            }
            els[b + 1] = keys; lens[b + 1] = keyl;
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "__rk_arr_min") || !strcmp(fn, "__rk_arr_max")) && nargs >= 1) {
        int want_max = (fn[10] == 'a');
        const char *best = NULL; size_t bestl = 0; long long bestn = 0; int best_num = 0; int have = 0;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
                char *ep; long long v = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
                int take;
                if (!have) take = 1;
                else if (isn && best_num) take = want_max ? (v > bestn) : (v < bestn);
                else { int c = strcmp(eb, best ? best : ""); take = want_max ? (c > 0) : (c < 0); }
                if (take) { char *cp = rt_ws_alloc(cl + 1); memcpy(cp, eb, cl); cp[cl] = '\0'; best = cp; bestl = cl; bestn = v; best_num = isn; have = 1; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (!have) { *out = NULVCL; return 1; }
        *out = best_num ? INTVAL(bestn) : STRVAL(rt_ws_strdup_c(best)); return 1;
    }
    if (!strcmp(fn, "__rk_arr_first") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        const char *nx = strchr(cs, SOH);
        size_t L = nx ? (size_t)(nx - cs) : strlen(cs);
        if (L == 0 && !nx && (!cs || !*cs)) { *out = NULVCL; return 1; }
        char *cp = rt_ws_alloc(L + 1); memcpy(cp, cs, L); cp[L] = '\0';
        char *ep; long long v = strtoll(cp, &ep, 10);
        *out = (*ep == '\0' && ep != cp) ? INTVAL(v) : STRVAL(cp); return 1;
    }
    if (!strcmp(fn, "__rk_arr_values") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        *out = STRVAL(rt_ws_strdup_c(cs ? cs : "")); return 1;
    }
    if (!strcmp(fn, "__rk_arr_keys") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        if (!cs || !*cs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int nel = 1; for (const char *p = cs; *p; p++) if (*p == SOH) nel++;
        char *buf = rt_ws_alloc((size_t)nel * 24 + 1); int p = 0;
        for (int i = 0; i < nel; i++) { if (i) buf[p++] = SOH; char nb[24]; int L = snprintf(nb, sizeof nb, "%d", i); memcpy(buf + p, nb, (size_t)L); p += L; }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "sum") && nargs >= 1) {
        long long isum = 0; double rsum = 0.0; int any_real = 0;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
                char *ep; long long iv = strtoll(eb, &ep, 10);
                if (*ep == '\0' && ep > eb) { isum += iv; rsum += (double)iv; }
                else { double dv = strtod(eb, NULL); any_real = 1; rsum += dv; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = any_real ? REALVAL(rsum) : INTVAL(isum); return 1;
    }
    if ((!strcmp(fn, "__rk_reduce_add") || !strcmp(fn, "__rk_reduce_sub") || !strcmp(fn, "__rk_reduce_mul")) && nargs >= 1) {
        int is_add = !strcmp(fn + 12, "add"); int is_mul = !strcmp(fn + 12, "mul");
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = INTVAL(is_mul ? 1 : 0); return 1; }
        long long iacc = 0; double racc = 0.0; int any_real = 0; int first = 1;
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
            char *ep; long long iv = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
            double dv = isn ? (double)iv : strtod(eb, NULL); if (!isn) any_real = 1;
            if (first) { iacc = iv; racc = dv; first = 0; }
            else if (is_add) { iacc += iv; racc += dv; }
            else if (is_mul) { iacc *= iv; racc *= dv; }
            else { iacc -= iv; racc -= dv; }
            if (!nx) break; seg = nx + 1;
        }
        *out = any_real ? REALVAL(racc) : INTVAL(iacc); return 1;
    }
    if (!strcmp(fn, "__rk_reduce_cat") && nargs >= 1) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *buf = rt_ws_strdup(""); size_t blen = 0; const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char *no = rt_ws_alloc(blen + L + 1); memcpy(no, buf, blen); memcpy(no + blen, seg, L); no[blen + L] = '\0';
            buf = no; blen += L;
            if (!nx) break; seg = nx + 1;
        }
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "__rk_reduce_min") || !strcmp(fn, "__rk_reduce_max")) && nargs >= 1) {
        int want_max = !strcmp(fn + 12, "max");
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = NULVCL; return 1; }
        const char *best = NULL; long long bestn = 0; int best_num = 0; int have = 0; const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
            char *ep; long long v = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
            int take; if (!have) take = 1; else if (isn && best_num) take = want_max ? (v > bestn) : (v < bestn);
            else { int c = strcmp(eb, best ? best : ""); take = want_max ? (c > 0) : (c < 0); }
            if (take) { char *cp = rt_ws_alloc(cl + 1); memcpy(cp, eb, cl); cp[cl] = '\0'; best = cp; bestn = v; best_num = isn; have = 1; }
            if (!nx) break; seg = nx + 1;
        }
        *out = best_num ? INTVAL(bestn) : STRVAL(rt_ws_strdup_c(best ? best : "")); return 1;
    }
    if (!strcmp(fn, "join") && nargs >= 1) {
        char sb[64]; const char *sep = to_cstring(args[0], sb, sizeof sb);
        char *sepd = rt_ws_strdup(sep ? sep : ""); size_t seplen = strlen(sepd);
        char *buf = rt_ws_strdup(""); size_t blen = 0; int first = 1;
        for (int i = 1; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                size_t add = (first ? 0 : seplen) + L;
                char *no = rt_ws_alloc(blen + add + 1);
                memcpy(no, buf, blen); size_t p = blen;
                if (!first) { memcpy(no + p, sepd, seplen); p += seplen; }
                memcpy(no + p, seg, L); p += L; no[p] = '\0';
                buf = no; blen = p; first = 0;
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "arr_make") && nargs == 1) {
        long long hi = IS_INT_fn(args[0]) ? args[0].i : 0; long long n = hi + 1; if (n < 1) n = 1;
        ARBLK_t *b = (ARBLK_t *) rt_ws_alloc_tag(sizeof(ARBLK_t), HB_ARR); b->id = rt_agg_serial_list(); b->lo = 0; b->hi = (int) hi;
        b->ndim = 1;
        b->ndim = 1;
        b->lo2 = 0; b->hi2 = 0; b->proto_bare = 0; b->data = (DESCR_t *) rt_ws_alloc(sizeof(DESCR_t) * (size_t) n); for (long long k = 0; k < n; k++) b->data[k] = INTVAL(0); DESCR_t d; d.v = DT_A;
        d.slen = 0; d.arr = b; *out = d; return 1;
    }
    if (!strcmp(fn, "arr_get") && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = FAILDESCR; return 1; } *out = b->data[i - b->lo]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (idx < 0 || !*cur) { *out = FAILDESCR; return 1; }
        if (idx >= 1 && !strchr(cur, SOH) && (size_t)idx <= strlen(cur)) { *out = INTVAL((long long)(unsigned char)cur[idx - 1]); return 1; }
        const char *seg = cur;
        long k = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) {
                size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                *out = elem_to_descr(seg, elen);
                return 1;
            }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
    }
    if (!strcmp(fn, "str_substr") || (!strcmp(fn, "substr") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        long slen = (long)strlen(s);
        long start = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (start < 0) start = slen + start;
        if (start < 0) start = 0;
        if (start > slen) start = slen;
        long len = slen - start;
        if (nargs >= 3) {
            long ll = IS_INT_fn(args[2]) ? args[2].i : len;
            if (ll < 0) ll = 0;
            if (start + ll > slen) ll = slen - start;
            len = ll;
        }
        char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, s + start, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "str_index") || (!strcmp(fn, "index") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : 0;
        if (from < 0) from = 0;
        if (*n == '\0') { *out = INTVAL(from); return 1; }
        const char *p = strstr(s + from, n);
        *out = p ? INTVAL((long)(p - s)) : INTVAL(-1); return 1;
    }
    if (!strcmp(fn, "str_rindex") || (!strcmp(fn, "rindex") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long slen = (long)strlen(s);
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : slen;
        size_t nlen = strlen(n);
        if (nlen == 0) { *out = INTVAL(from < slen ? from : slen); return 1; }
        long best = -1;
        for (long i = 0; i <= slen - (long)nlen && i <= from; i++)
            if (memcmp(s + i, n, nlen) == 0) best = i;
        *out = INTVAL(best); return 1;
    }
    if ((!strcmp(fn, "uc") || !strcmp(fn, "str_uc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = rt_ws_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "lc") || !strcmp(fn, "str_lc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = rt_ws_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'A' && s[i] <= 'Z') ? s[i] + 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "chars") || !strcmp(fn, "length")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = INTVAL((long)strlen(s)); return 1;
    }
    if ((!strcmp(fn, "str_trim") || !strcmp(fn, "trim")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
        size_t n = strlen(s);
        while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\n' || s[n-1] == '\r')) n--;
        char *o = rt_ws_alloc(n + 1); memcpy(o, s, n); o[n] = '\0';
        *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "sprintf") || !strcmp(fn, "__rk_sprintf")) && nargs >= 1) {
        char fb[512]; const char *fmt = to_cstring(args[0], fb, sizeof fb); if (!fmt) fmt = "";
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        *out = BSTRVAL(r, rl); return 1;
    }
    if ((!strcmp(fn, "printf") || !strcmp(fn, "__rk_printf")) && nargs >= 1) {
        char fb[512]; const char *fmt = to_cstring(args[0], fb, sizeof fb); if (!fmt) fmt = "";
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        fwrite(r, 1, rl, stdout); *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "array_sort") && nargs == 1) {
        const char *as = VARVAL_fn(args[0]); if (!as || !*as) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int cnt = 1; for (const char *p = as; *p; p++) if (*p == SOH) cnt++;
        char **elems = rt_ws_alloc((size_t)cnt * sizeof(char*));
        int idx = 0; const char *seg = as;
        do {
            const char *nx = strchr(seg, SOH);
            size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
            char *el = rt_ws_alloc(elen + 1); memcpy(el, seg, elen); el[elen] = '\0';
            elems[idx++] = el;
            seg = nx ? nx + 1 : NULL;
        } while (seg && idx < cnt);
        int all_int = 1;
        for (int i = 0; i < cnt && all_int; i++) {
            char *ep; strtol(elems[i], &ep, 10);
            if (*ep) all_int = 0;
        }
        if (all_int) {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; long kv = atol(key); int j = i - 1;
                while (j >= 0 && atol(elems[j]) > kv) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        } else {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; int j = i - 1;
                while (j >= 0 && strcmp(elems[j], key) > 0) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        }
        size_t total = 0; for (int i = 0; i < cnt; i++) total += strlen(elems[i]) + 1;
        char *o = rt_ws_alloc(total + 1); o[0] = '\0';
        for (int i = 0; i < cnt; i++) {
            if (i) { size_t ol = strlen(o); o[ol] = SOH; o[ol+1] = '\0'; }
            strcat(o, elems[i]);
        }
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "array_reverse") && nargs == 1) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; int n = b->hi - b->lo + 1; if (n < 0) n = 0;
            ARBLK_t *r = (ARBLK_t *) rt_ws_alloc_tag(sizeof(ARBLK_t), HB_ARR);
            r->id = rt_agg_serial_list(); r->lo = b->lo; r->hi = b->hi; r->ndim = 1; r->lo2 = 0; r->hi2 = 0; r->proto_bare = 0;
            r->data = (DESCR_t *) rt_ws_alloc(sizeof(DESCR_t) * (size_t) (n ? n : 1));
            for (int i = 0; i < n; i++) r->data[i] = b->data[n - 1 - i];
            DESCR_t d; d.v = DT_A; d.slen = 0; d.arr = r; *out = d; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        if (strchr(cur, SOH)) {
            int cnt = 1; for (const char *p = cur; *p; p++) if (*p == SOH) cnt++;
            char **elems = rt_ws_alloc((size_t)cnt * sizeof(char*));
            int idx = 0; const char *seg = cur;
            do {
                const char *nx = strchr(seg, SOH);
                size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                char *el = rt_ws_alloc(elen + 1); memcpy(el, seg, elen); el[elen] = '\0';
                elems[idx++] = el;
                seg = nx ? nx + 1 : NULL;
            } while (seg && idx < cnt);
            size_t total = 0; for (int i = 0; i < cnt; i++) total += strlen(elems[i]) + 1;
            char *o = rt_ws_alloc(total + 1); o[0] = '\0';
            for (int i = 0; i < cnt; i++) {
                if (i) { size_t ol = strlen(o); o[ol] = SOH; o[ol+1] = '\0'; }
                strcat(o, elems[cnt - 1 - i]);
            }
            *out = STRVAL(o); return 1;
        }
        *out = REVERS_fn(args[0]); return 1;
    }
    if ((!strcmp(fn, "die") || !strcmp(fn, "script_die")) && nargs >= 1) {
        const char *m = VARVAL_fn(args[0]); if (!m) m = "Died";
        extern void rt_script_die_surface(const char *msg);
        rt_script_die_surface(m);
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "srand") && nargs == 1) {
        long seed = IS_INT_fn(args[0]) ? (long)args[0].i : (IS_REAL_fn(args[0]) ? (long)args[0].r : 0);
        srand((unsigned int)seed);
        *out = INTVAL(seed); return 1;
    }
    if (!strcmp(fn, "callsame") || !strcmp(fn, "nextsame") || !strcmp(fn, "callwith")) {
        if (g_redisp_top <= 0) { *out = FAILDESCR; return 1; }
        RedispFrame *f = &g_redisp[g_redisp_top - 1];
        int next = -1;
        for (int i = f->found_idx + 1; i < f->mro_len; i++) {
            char cand[256]; snprintf(cand, sizeof cand, "%s__%s", f->mro[i], f->mname);
            if (meth_is_user_proc(cand)) { next = i; break; }
        }
        if (next < 0) { *out = FAILDESCR; return 1; }
        char nproc[256]; snprintf(nproc, sizeof nproc, "%s__%s", f->mro[next], f->mname);
        DESCR_t ca[16]; int total;
        if (!strcmp(fn, "callwith")) { total = 1 + nargs; if (total > 16) total = 16; ca[0] = f->self; for (int k = 0; k + 1 < total; k++) ca[1 + k] = args[k]; }
        else { total = f->nargs; if (total > 16) total = 16; for (int k = 0; k < total; k++) ca[k] = f->args[k]; }
        const char *smro[64]; int smro_len = f->mro_len < 64 ? f->mro_len : 64; for (int k = 0; k < smro_len; k++) smro[k] = f->mro[k];
        char smname[128]; snprintf(smname, sizeof smname, "%s", f->mname); DESCR_t sself = f->self;
        int rd = -1;
        if (g_redisp_top < 64) {
            rd = g_redisp_top++;
            g_redisp[rd].self = sself; snprintf(g_redisp[rd].mname, sizeof g_redisp[rd].mname, "%s", smname);
            for (int k = 0; k < smro_len; k++) g_redisp[rd].mro[k] = smro[k];
            g_redisp[rd].mro_len = smro_len; g_redisp[rd].found_idx = next;
            g_redisp[rd].nargs = total; for (int k = 0; k < total; k++) g_redisp[rd].args[k] = ca[k];
        }
        DESCR_t r = invoke_method_proc(nproc, ca, total);
        if (rd >= 0) g_redisp_top--;
        *out = r; return 1;
    }
    if (!strcmp(fn, "exc_clear") && nargs == 0) {
        extern char g_script_exception[512];
        g_script_exception[0] = '\0';
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "try_enter") && nargs == 0) {
        extern char g_script_exception[512]; extern int g_script_try_depth;
        g_script_try_depth++; g_script_exception[0] = '\0';
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "try_exit") && nargs == 0) {
        extern int g_script_try_depth;
        if (g_script_try_depth > 0) g_script_try_depth--;
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "exc_check") && nargs == 0) {
        extern char g_script_exception[512];
        if (g_script_exception[0] != '\0') { *out = INTVAL(1); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_get") && nargs == 0) {
        extern char g_script_exception[512];
        *out = STRVAL(rt_ws_strdup_c(g_script_exception)); return 1;
    }
    if (!strcmp(fn, "fh_capture") && nargs == 1) {
        extern void fh_ensure_init(void);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "open") && (nargs == 1 || nargs == 2)) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *mode = "r";
        if (nargs == 2) {
            const char *ms = VARVAL_fn(args[1]); if (!ms) ms = "";
            if (strstr(ms, ":w") || strstr(ms, "w")) mode = "w";
            else if (strstr(ms, ":a") || strstr(ms, "a")) mode = "a";
        }
        extern void fh_ensure_init(void);
        extern int fh_alloc(FILE *);
        fh_ensure_init();
        FILE *fp = fopen(path, mode);
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = INTVAL(idx); return 1;
    }
    if (!strcmp(fn, "close") && nargs == 1) {
        extern FILE *fh_get(int);
        extern void  fh_free(int);
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        FILE *fp = fh_get(idx);
        if (fp) { fclose(fp); fh_free(idx); }
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "slurp") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        fseek(fp, 0, SEEK_END); long sz = ftell(fp); rewind(fp);
        char *buf = rt_ws_alloc(sz + 1);
        size_t nr = fread(buf, 1, (size_t)sz, fp); buf[nr] = '\0';
        if (need_close) fclose(fp);
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "lines") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_alloc(65536); acc[0] = '\0'; size_t cap = 65536, used = 0; int first = 1;
        char line[4096];
        while (fgets(line, sizeof line, fp)) {
            size_t ll = strlen(line);
            while (ll > 0 && (line[ll-1] == '\n' || line[ll-1] == '\r')) line[--ll] = '\0';
            size_t need = used + ll + 2;
            if (need > cap) { cap = need * 2; char *nb = rt_ws_alloc(cap); memcpy(nb, acc, used); acc = nb; }
            if (!first) { acc[used++] = '\x01'; }
            memcpy(acc + used, line, ll); used += ll; acc[used] = '\0'; first = 0;
        }
        if (need_close) fclose(fp);
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "spurt") && nargs == 2) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *content = VARVAL_fn(args[1]); if (!content) content = "";
        FILE *fp = fopen(path, "w"); if (!fp) { *out = FAILDESCR; return 1; }
        fputs(content, fp); fclose(fp);
        *out = INTVAL(0); return 1;
    }
    if ((!strcmp(fn, "print_fh") || !strcmp(fn, "say_fh")) && nargs == 2) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) || IS_FH_fn(args[0]) ? args[0].i : 1);
        FILE *fp = fh_get(idx); if (!fp) fp = stdout;
        if (fp != stdout) fflush(stdout);
        char scratch[64]; const char *s = to_cstring(args[1], scratch, sizeof scratch);
        fputs(s, fp);
        if (!strcmp(fn, "say_fh")) fputc('\n', fp);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "field_get_pub") && nargs == 2) {
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        if (args[0].v == DT_DATA && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? di->type->name : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
        }
        extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
        *out = dat_field_get(fname, args[0]); return 1;
    }
    if (!strcmp(fn, "field_set") && nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        *out = args[2]; return 1;
    }
    if (!strcmp(fn, "field_set_pub") && nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        if (args[0].v == DT_DATA && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? di->type->name : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
            DatType *dt = cn ? dat_find_type(cn) : NULL;
            if (dt) { int isrw = 0, found = 0;
                for (int i = 0; i < dt->nfields; i++) if (!strcmp(dt->fields[i], fname)) { found = 1; isrw = dt->rw[i]; break; }
                if (found && !isrw) {
                    extern void rt_script_die_surface(const char *msg);
                    char _m[256]; snprintf(_m, sizeof _m, "Cannot modify an immutable attribute '$.%s' (declare it 'is rw' to allow assignment)", fname);
                    rt_script_die_surface(_m); *out = FAILDESCR; return 1; } }
        }
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        *out = args[2]; return 1;
    }
    if (!strcmp(fn, "obj_new") && nargs >= 1) {
        const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
        char newproc[256]; resolve_method_chain(cname, "new", newproc, sizeof newproc, NULL);
        extern int rt_proc_has_native_fn(const char *name);
        if (meth_is_user_proc(newproc)) {
            int pi;
            for (pi = 0; pi < g_stage2.proc_count; pi++)
                if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, newproc)) break;
            if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(newproc)) {
                extern DESCR_t g_call_args[];
                extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
                int total = nargs; for (int k = 0; k < total && k < 64; k++) g_call_args[k] = args[k];
                *out = rt_call_proc_descr(newproc, total); return 1;
            }
            extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
            *out = ir_call_proc(pi, args, nargs); return 1;
        }
        DatType *dt = dat_find_type(cname); if (!dt) { *out = FAILDESCR; return 1; }
        extern int dat_has_build_mro(const char *cls);
        if (dat_has_build_mro(cname)) {
            extern DESCR_t rt_construct_build(DatType *t, DESCR_t *named, int nnamed);
            *out = rt_construct_build(dt, &args[1], nargs - 1); return 1;
        }
        DESCR_t fvals[64];
        for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = NULVCL;
        for (int ci = 1; ci + 1 < nargs; ci += 2) {
            const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) {
                if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
        }
        *out = dat_construct(dt, fvals, dt->nfields); return 1;
    }
    if (!strcmp(fn, "meth_call") && nargs >= 2) {
        const char *mname0 = VARVAL_fn(args[1]);
        if (mname0 && mname0[0] == '^') {
            const char *mm = mname0 + 1; const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            if (cn && !strcmp(mm, "name")) { *out = STRVAL(rt_ws_strdup_c(cn)); return 1; }
            if (cn && !strcmp(mm, "parents")) {
                extern int dat_mro(const char *name, const char **out, int max); const char *mro[64]; int mn = dat_mro(cn, mro, 64);
                char buf[1024]; int pos = 0; buf[0] = 0;
                for (int i = 1; i < mn; i++) { if (!mro[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", mro[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            if (cn && !strcmp(mm, "methods")) {
                extern int dat_methods(const char *name, const char **out, int max); const char *ms[256]; int mn = dat_methods(cn, ms, 256);
                char buf[2048]; int pos = 0; buf[0] = 0;
                for (int i = 0; i < mn; i++) { if (!ms[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", ms[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            if (cn && !strcmp(mm, "attributes")) {
                extern int dat_attributes(const char *name, const char **out, int max); const char *as[256]; int an = dat_attributes(cn, as, 256);
                char buf[2048]; int pos = 0; buf[0] = 0;
                for (int i = 0; i < an; i++) { if (!as[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", as[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            *out = FAILDESCR; return 1;
        }
        if (mname0 && !strcmp(mname0, "WHAT")) {
            const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            if (cn) { *out = STRVAL(rt_ws_strdup_c(cn)); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (mname0 && (!strcmp(mname0, "isa") || !strcmp(mname0, "does")) && nargs >= 3) {
            const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            const char *target = VARVAL_fn(args[2]);
            if (!cn || !target) { *out = INTVAL(0); return 1; }
            extern int dat_mro(const char *name, const char **out, int max);
            extern int dat_roles(const char *name, const char **out, int max);
            int hit = 0; const char *mro[64]; int mn = dat_mro(cn, mro, 64);
            if (mn == 0) { mro[0] = cn; mn = 1; }
            for (int i = 0; i < mn && !hit; i++) if (mro[i] && !strcmp(mro[i], target)) hit = 1;
            if (!hit && !strcmp(mname0, "does")) {
                for (int i = 0; i < mn && !hit; i++) { const char *rls[8]; int rn = dat_roles(mro[i], rls, 8);
                    for (int j = 0; j < rn && !hit; j++) if (rls[j] && !strcmp(rls[j], target)) hit = 1; } }
            *out = INTVAL(hit ? 1 : 0); return 1;
        }
        if (mname0 && !strcmp(mname0, "bless")) {
            const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
            DatType *dt = dat_find_type(cname); if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[64];
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
                for (int fi = 0; fi < dt->nfields && fi < 64; fi++)
                    if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct(dt, fvals, dt->nfields); return 1;
        }
        if (mname0 && !strcmp(mname0, "clone") && args[0].v == DT_DATA && args[0].u) {
            DATINST_t *src = (DATINST_t *)args[0].u; const char *cname = (src && src->type) ? src->type->name : NULL;
            DatType *dt = cname ? dat_find_type(cname) : NULL; if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[64];
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = (src->fields && fi < src->type->nfields) ? src->fields[fi] : NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
                for (int fi = 0; fi < dt->nfields && fi < 64; fi++) if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct(dt, fvals, dt->nfields); return 1;
        }
        if (mname0 && !strcmp(mname0, "parse") && nargs == 3) {
            const char *gname = VARVAL_fn(args[0]);
            if (gname && rt_grammar_has_top(gname)) { const char *subj = VARVAL_fn(args[2]); return grammar_parse_core(gname, subj, out); }
        }
        if (nargs >= 2 && args[0].v != DT_DATA && rt_str_method(mname0, args[0], &args[2], nargs - 2, out)) return 1;
        if (args[0].v != DT_DATA && mname0) {
            const char *rtn = VARVAL_fn(args[0]);
            int is_dat_recv = (rtn && dat_find_type(rtn));
            if (!is_dat_recv && rtn && strchr(rtn, '\x02')) {
                int is_hashm = !strcmp(mname0, "keys") || !strcmp(mname0, "values") || !strcmp(mname0, "pairs") || !strcmp(mname0, "kv");
                if (is_hashm) {
                    const char *hfn = !strcmp(mname0, "keys") ? "hash_keys" : !strcmp(mname0, "values") ? "hash_values"
                                    : !strcmp(mname0, "pairs") ? "hash_pairs" : "hash_kv";
                    DESCR_t fa[1]; fa[0] = args[0];
                    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                    if (script_try_call_builtin_by_name(hfn, fa, 1, out)) return 1;
                }
            }
            if (!is_dat_recv) {
                int is_arrm = !strcmp(mname0, "reverse") || !strcmp(mname0, "unique") || !strcmp(mname0, "sort")
                           || !strcmp(mname0, "elems") || !strcmp(mname0, "end") || !strcmp(mname0, "join") || !strcmp(mname0, "sum")
                           || !strcmp(mname0, "head") || !strcmp(mname0, "tail") || !strcmp(mname0, "min")
                           || !strcmp(mname0, "max") || !strcmp(mname0, "first")
                           || !strcmp(mname0, "keys") || !strcmp(mname0, "values");
                if (is_arrm) {
                    const char *afn = !strcmp(mname0, "sort") ? "__rk_arr_sort" : !strcmp(mname0, "min") ? "__rk_arr_min"
                                    : !strcmp(mname0, "max") ? "__rk_arr_max" : !strcmp(mname0, "first") ? "__rk_arr_first"
                                    : !strcmp(mname0, "keys") ? "__rk_arr_keys" : !strcmp(mname0, "values") ? "__rk_arr_values"
                                    : !strcmp(mname0, "end") ? "elems" : mname0;
                    int total = 1 + (nargs - 2);
                    DESCR_t *fa = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
                    if (!strcmp(mname0, "join")) { for (int k = 0; k < nargs - 2; k++) fa[k] = args[2 + k]; fa[nargs - 2] = args[0]; }
                    else { fa[0] = args[0]; for (int k = 0; k < nargs - 2; k++) fa[1 + k] = args[2 + k]; }
                    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                    if (script_try_call_builtin_by_name(afn, fa, total, out)) {
                        if (!strcmp(mname0, "end") && IS_INT_fn(*out)) *out = INTVAL(out->i - 1);
                        return 1;
                    }
                }
            }
        }
        if (args[0].v != DT_DATA) {
            const char *tname = VARVAL_fn(args[0]);
            extern int rt_proc_has_native_fn(const char *name);
            if (tname && dat_find_type(tname)) {
                char tproc[256]; resolve_method_chain(tname, mname0, tproc, sizeof tproc, NULL);
                if (meth_is_user_proc(tproc)) {
                    int nextra = nargs - 2, total = 1 + nextra;
                    DESCR_t *ca = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
                    ca[0] = args[0]; for (int k = 0; k < nextra; k++) ca[1 + k] = args[2 + k];
                    int pi; for (pi = 0; pi < g_stage2.proc_count; pi++)
                        if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, tproc)) break;
                    if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(tproc)) {
                        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
                        for (int k = 0; k < total && k < 64; k++) g_call_args[k] = ca[k];
                        *out = rt_call_proc_descr(tproc, total); return 1;
                    }
                    extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
                    *out = ir_call_proc(pi, ca, total); return 1;
                }
            }
        }
        if (args[0].v != DT_DATA || !args[0].u) { *out = FAILDESCR; return 1; }
        DATINST_t *inst = (DATINST_t *)args[0].u;
        const char *cname = (inst && inst->type) ? inst->type->name : NULL;
        if (!cname) { *out = FAILDESCR; return 1; }
        const char *mname = VARVAL_fn(args[1]); if (!mname || !*mname) { *out = FAILDESCR; return 1; }
        char procname[256];
        int found_idx = -1;
        resolve_method_chain(cname, mname, procname, sizeof procname, &found_idx);
        if (!meth_is_user_proc(procname) && rt_multi_meth_dispatch(cname, mname, args, nargs, out)) return 1;
        if (nargs == 2 && !meth_is_user_proc(procname)) {
            extern int dat_field_is_private(const char *cls, const char *field);
            if (dat_field_is_private(cname, mname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", mname, cname);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
            extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
            DESCR_t *acc = data_field_ptr(mname, args[0]);
            if (acc) { *out = *acc; return 1; }
        }
        if (!meth_is_user_proc(procname)) {
            extern int dat_handles_field(const char *cls, const char *meth, char *out, int outsz);
            char delegfield[64];
            if (dat_handles_field(cname, mname, delegfield, sizeof delegfield)) {
                extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
                DESCR_t deleg = dat_field_get(delegfield, args[0]);
                DESCR_t *fwd = rt_ws_alloc((size_t)nargs * sizeof(DESCR_t)); fwd[0] = deleg; fwd[1] = args[1];
                for (int k = 2; k < nargs; k++) fwd[k] = args[k];
                extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                return script_try_call_builtin_by_name("meth_call", fwd, nargs, out);
            }
        }
        int nextra = nargs - 2;
        int total = 1 + nextra;
        DESCR_t *callargs = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
        callargs[0] = args[0];
        for (int k = 0; k < nextra; k++) callargs[1 + k] = args[2 + k];
        int rd = -1;
        if (g_redisp_top < 64) {
            rd = g_redisp_top++;
            extern int dat_mro(const char *name, const char **out, int max);
            g_redisp[rd].self = args[0]; snprintf(g_redisp[rd].mname, sizeof g_redisp[rd].mname, "%s", mname);
            g_redisp[rd].mro_len = dat_mro(cname, g_redisp[rd].mro, 64);
            if (g_redisp[rd].mro_len == 0) { g_redisp[rd].mro[0] = cname; g_redisp[rd].mro_len = 1; }
            g_redisp[rd].found_idx = found_idx;
            g_redisp[rd].nargs = total < 16 ? total : 16;
            for (int k = 0; k < g_redisp[rd].nargs; k++) g_redisp[rd].args[k] = callargs[k];
        }
        DESCR_t _mr = invoke_method_proc(procname, callargs, total);
        if (rd >= 0) g_redisp_top--;
        *out = _mr; return 1;
    }
    if (!strcmp(fn, "re_match") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = FAILDESCR; return 1; }
        nfa_exec(nfa, subj, &g_match);
        g_subject = subj;
        int verdict = g_match.matched ? 1 : 0;
        nfa_free(nfa);
        *out = verdict ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "nfa_accepts") && nargs == 2) {
        DESCR_t topic = args[0], matcher = args[1];
        if (junction_is(matcher)) {
            int numeric = (IS_INT_fn(topic) || IS_REAL_fn(topic));
            int ok = junction_collapse(topic, matcher, TT_EQ, numeric);
            *out = ok ? INTVAL(1) : FAILDESCR; return 1;
        }
        int both_num = (IS_INT_fn(topic) || IS_REAL_fn(topic)) && (IS_INT_fn(matcher) || IS_REAL_fn(matcher));
        int ok;
        if (both_num) {
            ok = (to_real(topic) == to_real(matcher));
        } else {
            const char *ts = VARVAL_fn(topic); if (!ts) ts = "";
            const char *ms = VARVAL_fn(matcher); if (!ms) ms = "";
            ok = (strcmp(ts, ms) == 0);
        }
        *out = ok ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "grammar_register") && nargs == 3) {
        const char *qn   = VARVAL_fn(args[0]);
        const char *body = VARVAL_fn(args[1]);
        const char *flv  = VARVAL_fn(args[2]);
        if (qn && body) gram_set(qn, body, flv ? atoi(flv) : 0);
        *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "grammar_parse") && nargs == 2) {
        const char *gname = VARVAL_fn(args[0]);
        const char *subj  = VARVAL_fn(args[1]);
        return grammar_parse_core(gname, subj, out);
    }
    if (!strcmp(fn, "re_match_global") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int slen = (int)strlen(subj);
        char *acc = rt_ws_alloc((size_t)slen * 4 + 4); acc[0] = '\0';
        int pos = 0, count = 0;
        while (pos <= slen) {
            Match m; nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) break;
            int mlen = m.full_end - m.full_start;
            if (count > 0) { int ol = (int)strlen(acc); acc[ol] = SOH; acc[ol + 1] = '\0'; }
            strncat(acc, subj + pos + m.full_start, (size_t)mlen);
            g_match = m;
            g_match.full_start += pos;
            g_match.full_end   += pos;
            for (int g = 0; g < m.ngroups; g++) {
                if (m.group_start[g] >= 0) g_match.group_start[g] += pos;
                if (m.group_end[g]   >= 0) g_match.group_end[g]   += pos;
            }
            g_subject = subj;
            pos += m.full_start + (mlen > 0 ? mlen : 1);
            count++;
        }
        nfa_free(nfa);
        *out = count > 0 ? STRVAL(acc) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "re_subst") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *tok  = VARVAL_fn(args[1]); if (!tok)  tok  = "";
        const char *sep1 = strchr(tok, SOH);
        if (!sep1) { *out = args[0]; return 1; }
        const char *sep2 = strchr(sep1 + 1, SOH);
        if (!sep2) { *out = args[0]; return 1; }
        int plen = (int)(sep1 - tok);
        int rlen = (int)(sep2 - (sep1 + 1));
        char *pat  = rt_ws_alloc((size_t)plen + 1); memcpy(pat, tok, (size_t)plen); pat[plen] = '\0';
        char *repl = rt_ws_alloc((size_t)rlen + 1); memcpy(repl, sep1 + 1, (size_t)rlen); repl[rlen] = '\0';
        int global = (*(sep2 + 1) == 'g');
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = args[0]; return 1; }
        int slen = (int)strlen(subj);
        char *res = rt_ws_alloc((size_t)slen * 4 + (size_t)rlen * 8 + 4); res[0] = '\0';
        int pos = 0, did_one = 0;
        while (pos <= slen) {
            Match m; nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
            strncat(res, subj + pos, (size_t)m.full_start);
            strcat(res, repl);
            g_match = m; g_subject = subj;
            int advance = m.full_start + (m.full_end - m.full_start > 0 ? m.full_end - m.full_start : 1);
            pos += advance; did_one = 1;
            if (!global) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
        }
        nfa_free(nfa);
        *out = did_one ? STRVAL(res) : args[0]; return 1;
    }
    if (!strcmp(fn, "nfa_compile") && nargs == 1) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) pat = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { printf("NFA:%s:ERROR\n", pat); *out = INTVAL(0); return 1; }
        printf("NFA:%s:states=%d\n", pat, nfa_state_count(nfa));
        nfa_free(nfa);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "re_capture") && nargs == 1) {
        int n = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        if (!g_match.matched || n < 0 || n >= g_match.ngroups || g_match.group_start[n] < 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int gs = g_match.group_start[n], ge = g_match.group_end[n];
        if (ge < gs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int len = ge - gs; char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "re_named_capture") && nargs == 1) {
        const char *name = VARVAL_fn(args[0]); if (!name) name = "";
        if (!g_match.matched) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int g = -1;
        for (int i = 0; i < g_match.ngroups; i++) if (strcmp(g_match.group_name[i], name) == 0) { g = i; break; }
        if (g < 0 || g_match.group_start[g] < 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int gs = g_match.group_start[g], ge = g_match.group_end[g];
        if (ge < gs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int len = ge - gs; char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "push_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = rt_ws_strdup(cur);
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = rt_ws_alloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "unshift_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = rt_ws_strdup("");
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = rt_ws_alloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        if (!*cur) { *out = STRVAL(acc); return 1; }
        size_t al = strlen(acc), cl = strlen(cur);
        char *o = rt_ws_alloc(al + 1 + cl + 1);
        memcpy(o, acc, al); o[al] = SOH; memcpy(o + al + 1, cur, cl); o[al + 1 + cl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_set_pure") && nargs >= 3) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = FAILDESCR; return 1; } b->data[i - b->lo] = args[2];
            *out = args[0]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        size_t rvl = strlen(rv);
        if (!strchr(cur, SOH)) {
            size_t slen = strlen(cur);
            if (idx == 0) { char *e0 = rt_ws_alloc(rvl + 1); memcpy(e0, rv, rvl); e0[rvl] = '\0'; *out = STRVAL(e0); return 1; }
            if (idx < 1 || (size_t)idx > slen) { *out = FAILDESCR; return 1; }
            char *buf = rt_ws_alloc(slen * 5 + rvl + 4);
            size_t pos = 0; buf[pos++] = '0';
            for (size_t j = 1; j <= slen; j++) {
                buf[pos++] = SOH;
                if ((long)j == idx) { memcpy(buf + pos, rv, rvl); pos += rvl; }
                else { int od = (unsigned char)cur[j-1]; pos += (size_t)sprintf(buf + pos, "%d", od); }
            }
            buf[pos] = '\0'; *out = STRVAL(buf); return 1;
        }
        const char *seg = cur;
        long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { tstart = seg; tend = nx; break; }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
        size_t pre = (size_t)(tstart - cur);
        size_t post = tend ? strlen(tend) : 0;
        size_t total = pre + rvl + post;
        char *o = rt_ws_alloc(total + 1);
        memcpy(o, cur, pre);
        memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post);
        o[total] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_last") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = FAILDESCR; return 1; }
        const char *last = strrchr(cur, SOH);
        const char *pstart = last ? last + 1 : cur;
        *out = elem_to_descr(pstart, strlen(pstart)); return 1;
    }
    if (!strcmp(fn, "arr_init") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *last = strrchr(cur, SOH);
        if (!last) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        size_t nl = (size_t)(last - cur);
        char *o = rt_ws_alloc(nl + 1); memcpy(o, cur, nl); o[nl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_tail") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *first = strchr(cur, SOH);
        if (!first) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        *out = STRVAL(rt_ws_strdup_c(first + 1)); return 1;
    }
    if (!strcmp(fn, "hash_set_pure") && nargs >= 3) {
        extern char *script_hash_set_str(const char *h, const char *key, const char *val);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        char vb[64]; const char *val = to_cstring(args[2], vb, sizeof vb);
        *out = STRVAL(script_hash_set_str(cur, key, val)); return 1;
    }
    if (!strcmp(fn, "hash_delete_pure") && nargs >= 2) {
        extern char *script_hash_delete_str(const char *h, const char *key);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = STRVAL(script_hash_delete_str(cur, key)); return 1;
    }
    extern int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
    if (script_try_hash_builtin(fn, args, nargs, out)) return 1;
    return 0;
}
#define STX '\x02'
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *hash_find(const char *h, const char *key, const char **p_pair_start) {
    if (!h || !key) return NULL;
    size_t klen = strlen(key);
    const char *seg = h;
    while (*seg) {
        const char *nx   = strchr(seg, SOH);
        size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
        const char *stx  = (const char *)memchr(seg, STX, plen);
        if (stx) {
            size_t this_klen = (size_t)(stx - seg);
            if (this_klen == klen && memcmp(seg, key, klen) == 0) {
                if (p_pair_start) *p_pair_start = seg;
                return stx + 1;
            }
        }
        if (!nx) break;
        seg = nx + 1;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_set_str(const char *h, const char *key, const char *val) {
    if (!h) h = "";
    size_t klen = strlen(key), vlen = strlen(val);
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (pair_start) {
        const char *nx = strchr(pair_start, SOH);
        size_t pre  = (size_t)(pair_start - h);
        size_t post = nx ? strlen(nx) : 0;
        size_t total = pre + klen + 1 + vlen + post;
        char *o = rt_ws_alloc(total + 1);
        memcpy(o, h, pre);
        memcpy(o + pre, key, klen);
        o[pre + klen] = STX;
        memcpy(o + pre + klen + 1, val, vlen);
        if (nx) memcpy(o + pre + klen + 1 + vlen, nx, post);
        o[total] = '\0'; return o;
    }
    size_t hlen = strlen(h);
    int need_sep = (hlen > 0);
    size_t total = hlen + (need_sep ? 1 : 0) + klen + 1 + vlen;
    char *o = rt_ws_alloc(total + 1);
    memcpy(o, h, hlen);
    if (need_sep) o[hlen] = SOH;
    memcpy(o + hlen + (need_sep ? 1 : 0), key, klen);
    o[hlen + (need_sep ? 1 : 0) + klen] = STX;
    memcpy(o + hlen + (need_sep ? 1 : 0) + klen + 1, val, vlen);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_delete_str(const char *h, const char *key) {
    if (!h || !*h) return rt_ws_strdup("");
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (!pair_start) return rt_ws_strdup(h);
    const char *nx = strchr(pair_start, SOH);
    size_t pre = (size_t)(pair_start - h);
    size_t trim_pre = pre;
    if (pre > 0) trim_pre = pre - 1;
    else if (nx) { nx++; }
    size_t post = nx ? strlen(nx) : 0;
    size_t total = trim_pre + post;
    char *o = rt_ws_alloc(total + 1);
    if (pre > 0) { memcpy(o, h, trim_pre); if (nx) memcpy(o + trim_pre, nx, post); }
    else if (nx) memcpy(o, nx, post);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || nargs < 1) return 0;
    if (args[0].v != DT_S && args[0].v != DT_SNUL) return 0;
    const char *h = VARVAL_fn(args[0]); if (!h) h = "";
    if (!strcmp(fn, "hash_get") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        const char *vstart = hash_find(h, key, NULL);
        if (!vstart) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *vend = strchr(vstart, SOH);
        size_t vlen = vend ? (size_t)(vend - vstart) : strlen(vstart);
        char *v = rt_ws_alloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
        char *ep; long iv = strtol(v, &ep, 10);
        if (*ep == '\0' && ep > v) { *out = INTVAL(iv); return 1; }
        *out = STRVAL(v); return 1;
    }
    if (!strcmp(fn, "hash_exists") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = hash_find(h, key, NULL) ? INTVAL(1) : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "hash_keys") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                char *k = rt_ws_alloc(klen + 1); memcpy(k, seg, klen); k[klen] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + klen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, k, klen); na[al + klen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_values") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                char *v = rt_ws_alloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + vlen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, v, vlen); na[al + vlen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_pairs") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                size_t pl = klen + 1 + vlen;
                char *pair = rt_ws_alloc(pl + 1);
                memcpy(pair, seg, klen); pair[klen] = ':';
                memcpy(pair + klen + 1, vstart, vlen); pair[pl] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + pl + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, pair, pl); na[al + pl] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_kv") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + klen + 1 + vlen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, seg, klen); al += klen; na[al++] = SOH;
                memcpy(na + al, vstart, vlen); na[al + vlen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    return 0;
}
#undef STX
#undef SOH
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs) { return rt_call_arr_bl(fn, args, nargs, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    { static long _rspc = -1; if (_rspc == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _rspc = (ev && *ev && *ev != '0') ? 0 : -2; } if (_rspc >= 0) { void *rsp_now; __asm__ volatile ("mov %%rsp, %0" : "=r"(rsp_now)); _rspc++; fprintf(stderr, "[RSP] %ld fn='%s' rsp=%p\n", _rspc, fn ? fn : "(null)", rsp_now); fflush(stderr); } }
    if (g_core_errjmp_n >= 64) { DESCR_t r0 = rt_call_arr_impl(fn, args, nargs, bidlen); return r0; }
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; return FAILDESCR; }
    g_core_errjmp_n = my + 1;
    DESCR_t r = rt_call_arr_impl(fn, args, nargs, bidlen);
    g_core_errjmp_n = my;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    DESCR_t out = FAILDESCR;
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    extern int g_gc_pending;
    { static long _cac = -1; if (_cac == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _cac = (ev && *ev && *ev != '0') ? 0 : -2; } if (_cac >= 0) { extern int g_core_errjmp_n; _cac++; fprintf(stderr, "[CAC] %ld fn='%s' nargs=%d errjmp_n=%d\n", _cac, fn ? fn : "(null)", nargs, g_core_errjmp_n); fflush(stderr); } }
    { static int _gcik = -1; if (_gcik == -1) { const char *ev = getenv("SCRIP_DISPATCH_GC_INLINE"); _gcik = (ev && *ev == '0') ? 0 : 1; }
      if (!_gcik || g_gc_pending) rt_gc_point_arr(args, nargs, (const char **)0); }
    if (!fn) return out;
    if (fn[0] == 'S' && fn[1] == 'N' && !strcmp(fn, "SNO$NOFAIL")) { extern void rt_nofail_abort(void); rt_nofail_abort(); return out; }
    if (fn[0] == '$' && fn[1]) { if (script_try_call_builtin_by_name(fn, args, nargs, &out)) return out; out = FAILDESCR; }
    if (fn[0] && !((fn[0] >= 'a' && fn[0] <= 'z') || (fn[0] >= 'A' && fn[0] <= 'Z') || fn[0] == '_' || fn[0] == '&')) {
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        if (nargs == 1) {
            extern DESCR_t rt_random_var(DESCR_t); extern DESCR_t rt_deref(DESCR_t);
            DESCR_t a = args[0];
            if (!strcmp(fn, "-"))  return rt_num_arith(INTVAL(0), a, BINOP_SUB);
            if (!strcmp(fn, "+"))  return rt_num_arith(INTVAL(0), a, BINOP_ADD);
            if (!strcmp(fn, "*"))  { extern DESCR_t rt_call_arr(const char *, DESCR_t *, int); DESCR_t _a = a; return try_call_builtin_by_name("*", &_a, 1, &out) ? out : FAILDESCR; }
            if (!strcmp(fn, "\\")) return (a.v == DT_SNUL || a.v == 0) ? FAILDESCR : a;
            if (!strcmp(fn, "?"))  return rt_deref(rt_random_var(a));
            if (!strcmp(fn, "/") || !strcmp(fn, "%") || !strcmp(fn, "#") || !strcmp(fn, "|")) {
                extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
                if (core_call_registered_fn(fn, args, nargs, &out)) return out;
                core_runtime_error(29, "undefined operator referenced");
                return FAILDESCR;
            }
        }
        DESCR_t a = (nargs > 0) ? args[0] : NULVCL, b = (nargs > 1) ? args[1] : NULVCL;
        if (!strcmp(fn, "[]")) { extern DESCR_t rt_subscript_var(DESCR_t, DESCR_t); extern DESCR_t rt_deref(DESCR_t); DESCR_t v = rt_subscript_var(a, b); if (IS_FAIL_fn(v)) return FAILDESCR; return rt_deref(v); }
        if (!strcmp(fn, "++")) return rt_num_arith(a, b, BINOP_CUNION);
        if (!strcmp(fn, "--")) return rt_num_arith(a, b, BINOP_CDIFF);
        if (!strcmp(fn, "**")) return rt_num_arith(a, b, BINOP_CINTER);
        if (!strcmp(fn, "+")) return rt_num_arith(a, b, BINOP_ADD);
        if (!strcmp(fn, "-")) return rt_num_arith(a, b, BINOP_SUB);
        if (!strcmp(fn, "*")) return rt_num_arith(a, b, BINOP_MUL);
        if (!strcmp(fn, "/")) return rt_num_arith(a, b, BINOP_DIV);
        if (!strcmp(fn, "%")) return rt_num_arith(a, b, BINOP_MOD);
        if (!strcmp(fn, "^")) return rt_num_arith(a, b, BINOP_POW);
        if (!strcmp(fn, "||")) { const char *x = VARVAL_fn(a), *y = VARVAL_fn(b); if (!x) x = ""; if (!y) y = ""; size_t lx = strlen(x), ly = strlen(y); char *o = rt_str_alloc((int)(lx + ly)); memcpy(o, x, lx); memcpy(o + lx, y, ly); o[lx + ly] = 0; return STRVAL(o); }
        { DESCR_t rt_str_coerce(DESCR_t); void rt_relop_val_coerce(DESCR_t, DESCR_t, DESCR_t *); int oc = -1;
          if      (!strcmp(fn, "="))    oc = BINOP_EQ;  else if (!strcmp(fn, "~="))   oc = BINOP_NE;
          else if (!strcmp(fn, "<"))    oc = BINOP_LT;  else if (!strcmp(fn, "<="))   oc = BINOP_LE;
          else if (!strcmp(fn, ">"))    oc = BINOP_GT;  else if (!strcmp(fn, ">="))   oc = BINOP_GE;
          else if (!strcmp(fn, "=="))   oc = BINOP_SEQ; else if (!strcmp(fn, "~=="))  oc = BINOP_SNE;
          else if (!strcmp(fn, "<<"))   oc = BINOP_SLT; else if (!strcmp(fn, "<<="))  oc = BINOP_SLE;
          else if (!strcmp(fn, ">>"))   oc = BINOP_SGT; else if (!strcmp(fn, ">>="))  oc = BINOP_SGE;
          else if (!strcmp(fn, "==="))  oc = BINOP_EQV; else if (!strcmp(fn, "~===")) oc = BINOP_NEQV;
          if (oc >= 0) { if (!rt_jct_relop(a, b, oc)) return FAILDESCR; if (oc >= BINOP_SLT && oc <= BINOP_SNE) return rt_str_coerce(b); DESCR_t _rv; rt_relop_val_coerce(a, b, &_rv); return _rv; } }
    }
    if (try_call_builtin_by_name_bl(fn, args, nargs, &out, bidlen)) return out;
    out = APPLY_fn(fn, args, nargs);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr_gen(const char *fn, DESCR_t *args, int nargs, int64_t *resume) {
    DESCR_t out = FAILDESCR;
    if (fn && resume && nargs >= 2 && nargs <= 4 && (!strcmp(fn, "find") || !strcmp(fn, "upto"))) {
        DESCR_t a4[4]; a4[0] = args[0]; a4[1] = args[1];
        long i1 = (nargs >= 3 && (IS_INT_fn(args[2]) || IS_REAL_fn(args[2]))) ? (long)to_int(args[2]) : 1;
        if (*resume > 0 && (long)*resume > i1) i1 = (long)*resume;
        a4[2] = INTVAL(i1);
        if (nargs >= 4) a4[3] = args[3];
        if (try_call_builtin_by_name(fn, a4, (nargs >= 4) ? 4 : 3, &out) && !IS_FAIL_fn(out)) { *resume = (int)out.i + 1; return out; }
        return FAILDESCR;
    }
    return rt_call_arr(fn, args, nargs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_list(DESCR_t *args, int nargs) {
    static int list_reg3 = 0;
    if (!list_reg3) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_reg3 = 1; }
    DESCR_t *elems = rt_ws_alloc((nargs>0?nargs:1)*sizeof(DESCR_t));
    for (int _j=0;_j<nargs;_j++) elems[_j]=args[_j];
    DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
    return DATCON_fn("list", eptr, INTVAL(nargs), STRVAL("list"), INTVAL(nargs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_args_list_from(char **v, int n) {
    if (n < 0 || !v) n = 0;
    DESCR_t *tmp = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
    for (int _i=0;_i<n;_i++) tmp[_i] = STRVAL(v[_i]);
    return rt_make_list(tmp, n);
}
static DESCR_t g_main_args_descr;
static char **g_main_args_v;
static int g_main_args_n = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_main_args_stage(char **v, int n) { if (n < 0 || !v) n = 0; g_main_args_v = v; g_main_args_n = n; }
void rt_main_args_bind(void) { extern DESCR_t g_call_args[]; if (g_main_args_descr.v == DT_DATA) return; if (g_main_args_n < 0) rt_main_args_stage((char **)0, 0); g_main_args_descr = rt_args_list_from(g_main_args_v, g_main_args_n); if (!getenv("SCRIP_NO_MAIN_ARGS")) g_call_args[0] = g_main_args_descr; }
DESCR_t rt_main_args_fetch(void) { rt_main_args_bind(); return g_main_args_descr; }
int rt_main_args_count(void) { return g_main_args_n < 0 ? 0 : g_main_args_n; }
extern int junction_is(DESCR_t v);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int relop_num_coerce(DESCR_t v, DESCR_t *out) {
    if (v.v == DT_BIG) { *out = v; return 1; }
    if (IS_INT_fn(v) || IS_REAL_fn(v)) { *out = v; return 1; }
    const char *s = IS_STR_fn(v) ? v.s : IS_CSET_fn(v) ? v.s : (const char *)0;
    if (!s) return 0;
    const char *t = s; while (*t == ' ') t++; if (!*t) return 0;
    char *endi = 0, *endd = 0; long long iv = strtoll(t, &endi, 10); double dv = strtod(t, &endd);
    const char *e = (endd > endi) ? endd : endi; if (e == t) return 0;
    while (*e == ' ') e++; if (*e != '\0') return 0;
    *out = (endd > endi) ? REALVAL(dv) : INTVAL((int64_t)iv); return 1;
}
DESCR_t rt_str_coerce(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_str_coerce(DESCR_t d) {
    if (!IS_CSET_fn(d)) return d;
    const char *cp; int cl; if (!cset_resolve(d, &cp, &cl) || cl < 0) return d;
    char *b = rt_ws_alloc((size_t)cl + 1); memcpy(b, cp, (size_t)cl); b[cl] = 0;
    for (int i = 1; i < cl; i++) { char t = b[i]; int j = i - 1; while (j >= 0 && (unsigned char)b[j] > (unsigned char)t) { b[j+1] = b[j]; j--; } b[j+1] = t; }
    return BSTRVAL(b, cl);
}
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int c_rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    if (g_core_errjmp_n >= 64) return rt_jct_relop_impl(lhs, rhs, op);
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; return 0; }
    g_core_errjmp_n = my + 1;
    int r = rt_jct_relop_impl(lhs, rhs, op);
    g_core_errjmp_n = my;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op) {
    if (op == BINOP_EQV || op == BINOP_NEQV) {
        int eq = 0;
        int lcs = (lhs.v == DT_S && lhs.slen == 0xFFFFFFFFu), rcs = (rhs.v == DT_S && rhs.slen == 0xFFFFFFFFu);
        if (lhs.v == DT_E && rhs.v == DT_E && lhs.slen == 0xFFFFFFFEu && rhs.slen == 0xFFFFFFFEu) eq = (lhs.s && rhs.s && strcmp(lhs.s, rhs.s) == 0);
        else if (lhs.v == rhs.v && lhs.i == rhs.i) eq = 1;
        else if (lcs || rcs) {
            if (lcs && rcs) { const char *ca, *cb; int la, lb; eq = cset_resolve(lhs, &ca, &la) && cset_resolve(rhs, &cb, &lb) && la == lb && memcmp(ca, cb, (size_t)la) == 0; }
        }
        else if ((lhs.v == DT_S || lhs.v == DT_SNUL) && (rhs.v == DT_S || rhs.v == DT_SNUL)) {
            const char *a = lhs.s ? lhs.s : ""; const char *b = rhs.s ? rhs.s : "";
            long la = lhs.slen ? (long)lhs.slen : (long)strlen(a); long lb = rhs.slen ? (long)rhs.slen : (long)strlen(b);
            eq = (la == lb) && memcmp(a, b, (size_t)la) == 0;
        }
        else if (lhs.v == rhs.v && lhs.v == DT_I) eq = (lhs.i == rhs.i);
        else if (lhs.v == rhs.v && lhs.v == DT_R) eq = (lhs.r == rhs.r);
        return (op == BINOP_EQV) ? eq : !eq;
    }
    int lj = junction_is(lhs), rj = junction_is(rhs);
    int num_rel = (op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT || op == BINOP_LE || op == BINOP_GT || op == BINOP_GE);
    int str_rel = (op == BINOP_SEQ || op == BINOP_SNE || op == BINOP_SLT || op == BINOP_SLE || op == BINOP_SGT || op == BINOP_SGE);
    if ((lj || rj) && (num_rel || str_rel)) {
        DESCR_t jct    = lj ? lhs : rhs;
        DESCR_t scalar = lj ? rhs : lhs;
        int tt_op = (op == BINOP_EQ || op == BINOP_SEQ) ? TT_EQ : (op == BINOP_NE || op == BINOP_SNE) ? TT_NE :
                    (op == BINOP_LT || op == BINOP_SLT) ? TT_LT : (op == BINOP_LE || op == BINOP_SLE) ? TT_LE :
                    (op == BINOP_GT || op == BINOP_SGT) ? TT_GT : TT_GE;
        int numeric = str_rel ? 0 : (IS_INT_fn(scalar) || IS_REAL_fn(scalar));
        return junction_collapse(scalar, jct, tt_op, numeric) ? 1 : 0;
    }
    if (num_rel) { DESCR_t L, R;
        int _relop_lok = relop_num_coerce(lhs, &L);
        if (_relop_lok && relop_num_coerce(rhs, &R)) {
            if (IS_REAL_fn(L) || IS_REAL_fn(R)) { double a = to_real(L), b = to_real(R);
                switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                              case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; } return 0; }
            if (L.v == DT_BIG || R.v == DT_BIG) { extern int rt_big_cmp(DESCR_t, DESCR_t); int c = rt_big_cmp(L, R);
                switch (op) { case BINOP_EQ: return c==0; case BINOP_NE: return c!=0; case BINOP_LT: return c<0;
                              case BINOP_LE: return c<=0; case BINOP_GT: return c>0;  case BINOP_GE: return c>=0; } return 0; }
            int64_t a = L.i, b = R.i;
            switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                          case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; } return 0; }
        core_icn_error(102, _relop_lok ? rhs : lhs); return 0; }
    if (num_rel && (IS_REAL_fn(lhs) || IS_REAL_fn(rhs)) && (IS_INT_fn(lhs) || IS_REAL_fn(lhs)) && (IS_INT_fn(rhs) || IS_REAL_fn(rhs))) {
        double a = to_real(lhs), b = to_real(rhs);
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    if (num_rel && IS_INT_fn(lhs) && IS_INT_fn(rhs)) {
        int64_t a = lhs.i, b = rhs.i;
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    { const char *a = VARVAL_fn(lhs), *b = VARVAL_fn(rhs); if (!a) a=""; if (!b) b="";
      size_t la = (lhs.v == DT_S && lhs.slen != 0xFFFFFFFFu && a == lhs.s) ? (size_t)lhs.slen : strlen(a);
      size_t lb = (rhs.v == DT_S && rhs.slen != 0xFFFFFFFFu && b == rhs.s) ? (size_t)rhs.slen : strlen(b);
      size_t lm = la < lb ? la : lb;
      int c = lm ? memcmp(a, b, lm) : 0;
      if (c == 0) c = (la < lb) ? -1 : (la > lb) ? 1 : 0;
      switch (op) { case BINOP_EQ: case BINOP_SEQ: return c==0; case BINOP_NE: case BINOP_SNE: return c!=0;
                    case BINOP_LT: case BINOP_SLT: return c<0;  case BINOP_LE: case BINOP_SLE: return c<=0;
                    case BINOP_GT: case BINOP_SGT: return c>0;  case BINOP_GE: case BINOP_SGE: return c>=0; }
      return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_real_str(double r, char *buf, int bufsz, int prec) {
    if (prec < 1) prec = 1; if (prec > 16) prec = 16;
    char tmp[64]; snprintf(tmp, sizeof tmp, "%.*E", prec, r);
    char *ep = strchr(tmp, 'E');
    if (!ep) { snprintf(buf, bufsz, "%s", tmp); return; }
    char sign = ep[1]; const char *digits = ep + 2; int ndig = (int)strlen(digits);
    char mant[48]; int ml = (int)(ep - tmp); if (ml >= 48) ml = 47; memcpy(mant, tmp, ml); mant[ml] = '\0';
    if (ndig < 3) snprintf(buf, bufsz, "%sE%c%0*d", mant, sign, 3, atoi(digits));
    else snprintf(buf, bufsz, "%s", tmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_as_value(const char *name) {
    if (!name || name[0] == '&') return FAILDESCR;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, name) == 0) {
            DESCR_t pv; pv.v = DT_E;
            pv.slen = (uint32_t)i;
            pv.i    = g_stage2.proc_table[i].entry_pc;
            return pv;
        }
    }
    static const char *const builtins[] = {
        "__pas_writeln","__pas_write","__pas_chr","__pas_chrlit","__pas_enum_name","__pas_read_i","__pas_read_c","__pas_readln","__pas_eof","__pas_eoln","__pas_trunc","__pas_abs","__pas_sin",
        "__pas_read_i_f","__pas_read_c_f","__pas_readln_f","__pas_eof_f","__pas_eoln_f","__pas_getbufch","__pas_getbufch_f",
        "__pas_ca_pack","__pas_ca_unpack",
        "__pas_cos","__pas_exp","__pas_sqrt","__pas_ln","__pas_arctan","__pas_fassign","__pas_rewrite","__pas_reset","__pas_fclose","write","writes","read","reads","close","open","remove",
        "flush",
        "put","get","pull","push","pop","list","image","proc","type","copy",
        "string","integer","real","numeric","ord","char","reverse","sort","sortf",
        "find","match","many","any","upto","bal","move","tab","pos",
        "map","repl","trim","left","right","center","detab","entab",
        "abs","sqrt","sin","cos","tan","asin","acos","atan","exp","log",
        "dtor","rtod",
        "iand","ior","ixor","ishift","icom",
        "table","key","insert","delete","member","args","level",
        "collect","stop","exit","runerr","name","variable","seq",
        NULL
    };
    for (int i = 0; builtins[i]; i++) if (strcmp(builtins[i], name) == 0) return STRVAL(name);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void out_write_str(FILE *dest, const char *s) {
    if (!s || !*s) return;
    if (s[0] == '\x03') {
        char flav = s[1];
        const char *tname = (flav == 'a') ? "any" : (flav == 'l') ? "all" : (flav == 'o') ? "one" : "none";
        fputs(tname, dest); fputc('(', dest);
        const char *p = s + 2; int first = 1;
        while (*p == '\x01') {
            p++;
            if (!first) fputs(", ", dest); first = 0;
            if (*p == '\x03') {
                int depth = 1; const char *start = p; p++;
                while (*p && depth > 0) { if (*p == '\x03') depth++; else if (*p == '\x04') depth--; p++; }
                size_t L = (size_t)(p - start);
                char *mb = rt_ws_alloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
                out_write_str(dest, mb);
            } else {
                while (*p && *p != '\x01' && *p != '\x04') { fputc((unsigned char)*p, dest); p++; }
            }
        }
        fputc(')', dest); return;
    }
    if (strchr(s, '\x01')) {
        const char *p = s; int first = 1;
        while (*p) {
            const char *seg = p;
            while (*p && *p != '\x01') p++;
            if (!first) fputc(' ', dest);
            fwrite(seg, 1, (size_t)(p - seg), dest);
            first = 0;
            if (*p == '\x01') p++;
        }
        return;
    }
    fputs(s, dest);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_obj_stringify(DESCR_t d, int use_gist) {
    if (d.v == DT_DATA && d.u && d.u->type && d.u->type->name) {
        const char *mname = use_gist ? "gist" : "Str";
        char proc[256]; resolve_method_chain(d.u->type->name, mname, proc, sizeof proc, NULL);
        if (meth_is_user_proc(proc)) { DESCR_t self1 = d; DESCR_t r = invoke_method_proc(proc, &self1, 1); const char *s = VARVAL_fn(r); return s ? s : ""; }
    }
    const char *s = VARVAL_fn(d); return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void out_write_descr(FILE *dest, DESCR_t av, int use_gist) {
    if (IS_INT_fn(av))  { fprintf(dest, "%lld", (long long)av.i); return; }
    if (IS_REAL_fn(av)) { char _rb[64]; fprintf(dest, "%s", icon_real_str(av.r,_rb,sizeof _rb)); return; }
    if (IS_CSET_fn(av)) { if (av.s) { int _kl = kw_cset_len(av.s); fwrite(av.s, 1, (_kl >= 0) ? (size_t)_kl : strlen(av.s), dest); } return; }
    if (av.v == (DTYPE_t)DT_PLREF || av.v == (DTYPE_t)DT_PLVAR) { extern void rt_pl_write_cell_fp(void *, FILE *); DESCR_t _pt = av; fflush(dest); rt_pl_write_cell_fp(plw_entry(&_pt), dest); return; }
    if (av.v == DT_DATA) { const char *s = rk_obj_stringify(av, use_gist); if (s) out_write_str(dest, s); return; }
    const char *s = VARVAL_fn(av); if (s) out_write_str(dest, s);
}
#define _OPCOERCE(d) do { \
        if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { \
            const char *_s = VARVAL_fn(d); \
            if (_s && *_s) { char *_e=NULL; long long _iv=strtoll(_s,&_e,10); \
                if (_e && !*_e){(d)=INTVAL(_iv);} \
                else {double _rv=strtod(_s,&_e); \
                      if(_e && !*_e){(d)=REALVAL(_rv);}else{*out=FAILDESCR;return 1;}} \
            } else { *out=FAILDESCR; return 1; } } } while(0)
#define _NUMREL(op) do { DESCR_t _l=args[0],_r=args[1]; _OPCOERCE(_l); _OPCOERCE(_r); \
        double _lv2=IS_REAL_fn(_l)?_l.r:(double)_l.i, _rv2=IS_REAL_fn(_r)?_r.r:(double)_r.i; \
        *out=(_lv2 op _rv2)?_r:FAILDESCR; return 1; } while(0)
#define _STRREL(op) do { DESCR_t _l=args[0],_r=args[1]; \
        const char *_ls=VARVAL_fn(_l); if(!_ls)_ls=""; \
        const char *_rs=VARVAL_fn(_r); if(!_rs)_rs=""; \
        int _cmp=strcmp(_ls,_rs); *out=(_cmp op 0)?rt_str_coerce(_r):FAILDESCR; return 1; } while(0)
#define _SNOCOERCE(d) do { \
        if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { \
            const char *_s9 = VARVAL_fn(d); \
            if (!_s9 || !*_s9) { (d) = INTVAL(0); } else { _OPCOERCE(d); } } } while(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_cvpos(long long pos, int len, int *out_p) {
    if (pos < -(long long)len || pos > (long long)len + 1) return 0;
    *out_p = (int)(pos > 0 ? pos : (long long)len + pos + 1);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_str_anal(DESCR_t *args, int nargs, int si, const char **out_s, int *out_i, int *out_j) {
    extern const char *scan_subj;
    extern int scan_pos;
    int have_s = (nargs >= si + 1) && !IS_FAIL_fn(args[si])     && args[si].v     != DT_SNUL;
    int have_i = (nargs >= si + 2) && !IS_FAIL_fn(args[si + 1]) && args[si + 1].v != DT_SNUL;
    int have_j = (nargs >= si + 3) && !IS_FAIL_fn(args[si + 2]) && args[si + 2].v != DT_SNUL;
    const char *s;
    int i, j, slen;
    if (!have_s) {
        if (!scan_subj) return 0;
        s = scan_subj;
        i = scan_pos;
    } else {
        s = VARVAL_fn(args[si]);
        if (!s) s = "";
        i = 1;
    }
    slen = (int)strlen(s);
    if (have_i && !bn_cvpos((long long)to_int(args[si + 1]), slen, &i)) return 0;
    if (!have_j) j = slen + 1;
    else {
        if (!bn_cvpos((long long)to_int(args[si + 2]), slen, &j)) return 0;
        if (i > j) { int t = i; i = j; j = t; }
    }
    if (i < 1 || i > slen + 1 || j < 1 || j > slen + 1) return 0;
    *out_s = s; *out_i = i; *out_j = j;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_numrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    DESCR_t _l = args[0], _r = args[1]; _SNOCOERCE(_l); _SNOCOERCE(_r);
    double a = IS_REAL_fn(_l) ? _l.r : (double)_l.i, b = IS_REAL_fn(_r) ? _r.r : (double)_r.i;
    int ok = op==0?(a==b):op==1?(a!=b):op==2?(a<b):op==3?(a<=b):op==4?(a>b):(a>=b);
    *out = ok ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_lexrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    int c = lex_cmp_pair(args[0], args[1]);
    int ok = op==0?(c>0):op==1?(c<0):op==2?(c>=0):op==3?(c<=0):op==4?(c==0):(c!=0);
    *out = ok ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_identdiffer(DESCR_t *args, int nargs, DESCR_t *out, int ident) {
    extern int descr_identical(DESCR_t, DESCR_t);
    DESCR_t a = nargs > 0 ? args[0] : NULVCL, b = nargs > 1 ? args[1] : NULVCL;
    int same = descr_identical(a, b);
    *out = (ident ? same : !same) ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_size(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t v = args[0];
    if (IS_FAIL_fn(v)) { *out = FAILDESCR; return 1; }
    if (v.v == DT_T)   { *out = INTVAL(v.tbl ? v.tbl->size : 0); return 1; }
    if (v.v == DT_A)   { *out = INTVAL(v.arr ? (v.arr->hi - v.arr->lo + 1) : 0); return 1; }
    if (v.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(v,"gen_type");
        if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0) {
            *out = INTVAL((int)FIELD_GET_fn(v,"frame_size").i); return 1;
        }
    }
    if (v.v == DT_I) { const char *is = VARVAL_fn(v); *out = INTVAL(is ? (long)strlen(is) : 0); return 1; }
    if (v.v == DT_R) { *out = INTVAL(0); return 1; }
    if (v.v == DT_S && v.slen == 0xFFFFFFFFu) {
        int klen = kw_cset_len(v.s);
        *out = INTVAL(klen >= 0 ? klen : (v.s ? (long)strlen(v.s) : 0));
        return 1;
    }
    const char *s = VARVAL_fn(v); if (!s) { *out = INTVAL(0); return 1; }
    if (strchr(s,'\x01')) {
        long n=1; for(const char *p=s;*p;p++) if(*p=='\x01') n++;
        *out = INTVAL(n); return 1;
    }
    long len = v.slen > 0 ? v.slen : (long)strlen(s);
    *out = INTVAL(len); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_time(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)args; if (nargs > 1) return -1;
    *out = INTVAL(rt_time_ns()); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_date(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)args; if (nargs > 1) return -1;
    time_t now = time(NULL); struct tm *tm = localtime(&now);
    char *buf = (char *)rt_ws_alloc_c(64); strftime(buf, 64, "%m/%d/%Y %H:%M:%S", tm);
    *out = STRVAL(buf); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_trim(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    const char *sv = VARVAL_fn(args[0]); if (!sv) sv = "";
    size_t n = sv_len(args[0], sv); while (n > 0 && sv[n-1] == ' ') n--;
    char *buf = (char *)rt_ws_alloc_c(n + 1); memcpy(buf, sv, n); buf[n] = 0;
    *out = BSTRVAL(buf, n); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_dupl(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    const char *sv = VARVAL_fn(args[0]); if (!sv) sv = "";
    DESCR_t nn = args[1]; _SNOCOERCE(nn);
    long long k = IS_REAL_fn(nn) ? (long long)nn.r : nn.i;
    if (k < 0) { *out = FAILDESCR; return 1; }
    size_t sl = sv_len(args[0], sv); char *buf = (char *)rt_ws_alloc_c(sl * (size_t)k + 1);
    for (long long i = 0; i < k; i++) memcpy(buf + (size_t)i * sl, sv, sl);
    buf[sl * (size_t)k] = 0; *out = BSTRVAL(buf, sl * (size_t)k); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int repl_pl_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_REPL_PL"); v = (e && *e == '0') ? 1 : 0; } return v; }
void c_rt_translate_bytes(char *dst, const char *src, size_t n, const char *map) {
    for (size_t i = 0; i < n; i++) dst[i] = map[(unsigned char)src[i]];
}
#define BN_PTRLEN(A, P, L, F) do { DESCR_t _a = (A); \
    if ((F) && _a.v == DT_S && _a.s && _a.slen != 0xFFFFFFFFu && _a.slen != 0u) { (P) = _a.s; (L) = (size_t)_a.slen; } \
    else { (P) = VARVAL_fn(_a); if (!(P)) (P) = ""; (L) = sv_len(_a, (P)); } } while (0)
static __attribute__((noinline)) int bn_replace(DESCR_t *args, int nargs, DESCR_t *out) {
    static struct { unsigned char v, n; char f[63], t[63], map[256]; } g_rm[4];
    static int g_rm_off = -1;
    if (nargs != 3) return -1;
    const char *sv, *fv, *tv; size_t sl, fl, tl;
    const int _plf = !repl_pl_off();
    BN_PTRLEN(args[0], sv, sl, _plf);
    BN_PTRLEN(args[1], fv, fl, _plf);
    BN_PTRLEN(args[2], tv, tl, _plf);
    if (fl != tl || !fl) { *out = FAILDESCR; return 1; }
    if (g_rm_off < 0) { const char *e = getenv("SCRIP_REPLMAP_OFF"); g_rm_off = (e && *e) ? 1 : 0; }
    char mloc[256]; char *map = mloc;
    if (!g_rm_off && fl < 63) { unsigned s = ((unsigned char)fv[0] * 31u + (unsigned)fl) & 3u;
        if (g_rm[s].v && g_rm[s].n == (unsigned char)fl && !memcmp(g_rm[s].f, fv, fl) && !memcmp(g_rm[s].t, tv, fl)) map = g_rm[s].map;
        else { map = g_rm[s].map; for (int i = 0; i < 256; i++) map[i] = (char)i; for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k];
               memcpy(g_rm[s].f, fv, fl); memcpy(g_rm[s].t, tv, fl); g_rm[s].n = (unsigned char)fl; g_rm[s].v = 1; } }
    else { for (int i = 0; i < 256; i++) map[i] = (char)i; for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k]; }
    size_t n = sl; char *buf = (char *)rt_ws_alloc_c(n + 1);
    rt_translate_bytes(buf, sv, n, map);
    buf[n] = 0; *out = BSTRVAL(buf, n); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_substr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = SUBSTR_fn(args[0], args[1], (nargs == 3) ? args[2] : INTVAL(0)); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_reverse(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    *out = REVERS_fn(args[0]); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_lpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = lpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" ")); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_rpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = rpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" ")); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_integer(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t av = args[0];
    if (IS_INT_fn(av))  { *out = NULVCL; return 1; }
    if (IS_REAL_fn(av)) { *out = FAILDESCR; return 1; }
    const char *sv = VARVAL_fn(av); if (!sv) sv = "";
    const char *p = sv; while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0') { *out = NULVCL; return 1; }
    if (*p == '+' || *p == '-') p++;
    const char *d = p; while (*p >= '0' && *p <= '9') p++;
    const char *e = p; while (*e == ' ' || *e == '\t') e++;
    *out = (p != d && *e == '\0') ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_remdr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    DESCR_t a = args[0], b = args[1]; _SNOCOERCE(a); _SNOCOERCE(b);
    if (IS_REAL_fn(a) || IS_REAL_fn(b)) {
        double rb = IS_REAL_fn(b) ? b.r : (double)b.i;
        if (rb == 0.0) { *out = FAILDESCR; return 1; }
        double rr = fmod(IS_REAL_fn(a) ? a.r : (double)a.i, rb);
        if (isinf(rr)) { core_runtime_error(312, "remdr caused real overflow"); *out = FAILDESCR; return 1; }
        *out = REALVAL(rr); return 1;
    }
    long long ai = a.i, bi = b.i;
    if (bi == 0) { *out = FAILDESCR; return 1; }
    *out = INTVAL(ai % bi); return 1;
}
extern unsigned rt_dtax_gen;
typedef struct { unsigned gen; unsigned char len; signed char kind; short nf; char nm[14]; void *ctor; const char *syn; } dtax_ent_t;
static dtax_ent_t g_dtax[256];
static dtax_ent_t g_dtax_bid[1025];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dtax_off(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_DTAX_OFF"); p = (e && *e) ? 1 : 0; } return p; }
static void dtx4(dtax_ent_t *e, unsigned char l, const char *nm, void *h) { if (!e || !l) return; e->gen = rt_dtax_gen; e->len = l; e->kind = 4; e->nf = 0; memcpy(e->nm, nm, l); e->ctor = h; e->syn = 0; }
static void dtx5(dtax_ent_t *e, unsigned char l, const char *nm, void *h, short x) { if (!e || !l) return; e->gen = rt_dtax_gen; e->len = l; e->kind = 5; e->nf = x; memcpy(e->nm, nm, l); e->ctor = h; e->syn = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_type_datatype(const char *fn, DESCR_t *args, int nargs, DESCR_t *out)
{
    DESCR_t av = args[0];
    const char *t;
    int declared = 0;
    (void)nargs;
    if (av.v == DT_BIG)      t="integer";
    else if (IS_INT_fn(av))  t="integer";
    else if (IS_REAL_fn(av)) t="real";
    else if (av.v==DT_T)     t=(av.tbl && av.tbl->is_set) ? "set" : "table";
    else if (av.v==DT_A)     t=(!strcmp(fn,"DATATYPE")) ? "array" : "list";
    else if (av.v==DT_DATA)  {
        DESCR_t tag = FIELD_GET_fn(av,"gen_type");
        if (tag.v==DT_S && tag.s) t = tag.s;
        else { DATINST_t *di = (DATINST_t *)av.u; declared = (di && di->type && di->type->name) ? 1 : 0; t = declared ? di->type->name : "record"; }
    }
    else if (IS_CSET_fn(av)) t="cset";
    else if (IS_FH_fn(av))   t="file";
    else if (av.v==DT_E) {
        t = "procedure";
        if (!strcmp(fn,"DATATYPE")) { t = "function";
            if (av.slen == 0xFFFFFFFEu && av.s) {
                for (int _ti=0;_ti<g_stage2.proc_count;_ti++) if (g_stage2.proc_table[_ti].name && !strcmp(g_stage2.proc_table[_ti].name,av.s)){t="procedure";break;}
                if (!strcmp(t,"function")) { extern int rt_proc_is_registered(const char *); if (rt_proc_is_registered(av.s)) t="procedure"; }
            } else t="procedure"; }
    }
    else if (av.v==DT_C)     t="CODE";
    else if (av.v==DT_X)     t="EXPRESSION";
    else if (av.v==DT_N)     t="name";
    else if (av.v==DT_P)     t="PATTERN";
    else if (av.v==DT_SNUL)  t=(!strcmp(fn,"DATATYPE")) ? "string" : "null";
    else t="string";
    if (!strcmp(fn,"DATATYPE") && declared) { *out = STRVAL(rt_ws_strdup_c(t)); return 1; }
    if (!strcmp(fn,"DATATYPE")) { char ub[32]; int ui=0;
        for (; t[ui] && ui<31; ui++) ub[ui]=(char)((t[ui]>='a'&&t[ui]<='z')?t[ui]-32:t[ui]); ub[ui]=0; *out = STRVAL(rt_ws_strdup_c(ub)); return 1; }
    *out = STRVAL(t); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rt_record_image_id(void *inst)
{
    return inst ? ((DATINST_t *)inst)->id : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_sno_name(DESCR_t *args, int nargs, DESCR_t *out)
{
    const char *sv = rt_sno_indirect_name(args[0]);
    (void)nargs;
    if (!sv || !*sv) { *out = FAILDESCR; return 1; }
    { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_N; d.slen = 0; d.s = rt_ws_strdup(sv); *out = d; return 1; }
}
long g_bidprof[1024]; int g_bidprof_on = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bidprof_dump(void) { for (int i = 0; i < 1024; i++) if (g_bidprof[i]) fprintf(stderr, "BIDPROF %d %ld\n", i, g_bidprof[i]); }
void bidprof_init(void) { const char *e = getenv("SCRIP_BID_PROF"); g_bidprof_on = (e && e[0]=='1') ? 1 : 0; if (g_bidprof_on) atexit(bidprof_dump); }
int g_bidjmp_on = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sort_key_cstr(DESCR_t v, char *buf, int bufsz) {
    if (v.v == DT_S)    return v.s ? v.s : "";
    if (v.v == DT_SNUL) return "";
    if (v.v == DT_I)    { snprintf(buf, (size_t)bufsz, "%lld", (long long)v.i); return buf; }
    { const char *s = VARVAL_fn(v); return s ? s : ""; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_descr_cmp(DESCR_t a, DESCR_t b) {
    if (IS_INT_fn(a) && IS_INT_fn(b)) return (a.i > b.i) ? 1 : (a.i < b.i) ? -1 : 0;
    { char ba[64], bb[64]; const char *sa = sort_key_cstr(a, ba, (int)sizeof ba), *sb = sort_key_cstr(b, bb, (int)sizeof bb); return strcmp(sa, sb); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_field_of(DESCR_t v, int field_idx) {
    if (field_idx >= 0 && v.v == DT_DATA && v.u) { DATINST_t *ia = (DATINST_t *)v.u; if (ia->type && field_idx < ia->type->nfields) return ia->fields[field_idx]; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_descr(DESCR_t *a, DESCR_t *tmp, int n, int field_idx) {
    if (n < 2) return;
    { int m = n / 2, i = 0, j = n / 2, k = 0, t;
      sort_msort_descr(a, tmp, m, field_idx); sort_msort_descr(a + m, tmp + m, n - m, field_idx);
      while (i < m && j < n) { if (sort_descr_cmp(sort_field_of(a[j], field_idx), sort_field_of(a[i], field_idx)) < 0) tmp[k++] = a[j++]; else tmp[k++] = a[i++]; }
      while (i < m) tmp[k++] = a[i++];
      while (j < n) tmp[k++] = a[j++];
      for (t = 0; t < n; t++) a[t] = tmp[t]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_pairs(TBPAIR_t **a, TBPAIR_t **tmp, int n, int by_val) {
    if (n < 2) return;
    { int m = n / 2, i = 0, j = n / 2, k = 0, t;
      sort_msort_pairs(a, tmp, m, by_val); sort_msort_pairs(a + m, tmp + m, n - m, by_val);
      while (i < m && j < n) { DESCR_t x = by_val ? a[i]->val : a[i]->key_descr, y = by_val ? a[j]->val : a[j]->key_descr; if (sort_descr_cmp(y, x) < 0) tmp[k++] = a[j++]; else tmp[k++] = a[i++]; }
      while (i < m) tmp[k++] = a[i++];
      while (j < n) tmp[k++] = a[j++];
      for (t = 0; t < n; t++) a[t] = tmp[t]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) { return try_call_builtin_by_name_bl(fn, args, nargs, out, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_argtype_gate(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs < 1 || IS_FAIL_fn(args[0])) return 0;
    switch (bid) {
        case BID_any: case BID_many: case BID_upto: return icn_cvt_chars_ok(args[0]) ? 0 : icn_argtype_raise(104, args[0], out);
        case BID_find: case BID_match:             return icn_cvt_chars_ok(args[0]) ? 0 : icn_argtype_raise(103, args[0], out);
        case BID_tab: case BID_move:               return icn_cvt_int_ok(args[0])   ? 0 : icn_argtype_raise(101, args[0], out);
        case BID_insert: case BID_member: return args[0].v == DT_T ? 0 : icn_argtype_raise(122, args[0], out);
        case BID_delete: return (args[0].v == DT_T || IS_STR_fn(args[0])) ? 0 : icn_argtype_raise(122, args[0], out);
        case BID_key: return (args[0].v == DT_T && args[0].tbl && !args[0].tbl->is_set) ? 0 : icn_argtype_raise(124, args[0], out);
        case BID_bal:
            for (int i = 0; i < 3 && i < nargs; i++)
                if (!IS_FAIL_fn(args[i]) && args[i].v != DT_SNUL && !icn_cvt_chars_ok(args[i])) return icn_argtype_raise(104, args[i], out);
            return 0;
        default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen)
{
    if (nargs == 1 && args[0].v == DT_DATA && args[0].u && args[0].u->type) {
        DATBLK_t *idb = args[0].u->type; const char _f0 = fn ? fn[0] : 0;
        for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) {
            extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
            *out = dat_field_get(fn, args[0]); return 1;
        }
    }
    if (!fn || !out) return 0;
    if (bidlen >= 0 && rt_dtax_gen == 0 && !dtax_off()) {
        const int _fb = bidlen & 0xFFFF;
        extern long g_bidprof[1024]; extern int g_bidprof_on; extern void bidprof_init(void);
        if (g_bidprof_on < 0) bidprof_init();
        if (g_bidprof_on && _fb >= 0 && _fb < 1024) g_bidprof[_fb]++;
        if (_fb == BID_SIZE)    return bn_size(args, nargs, out);
        if (_fb == BID_REPLACE) return bn_replace(args, nargs, out);
        if (_fb == BID_TRIM)    return bn_trim(args, nargs, out);
        if (_fb == BID_SUBSTR)  return bn_substr(args, nargs, out);
        if (_fb == BID_REVERSE) return bn_reverse(args, nargs, out);
        if (_fb == BID_INTEGER) return bn_integer(args, nargs, out);
        if (_fb == BID_DUPL)    return bn_dupl(args, nargs, out);
        if (_fb == BID_LPAD)    return bn_lpad(args, nargs, out);
        if (_fb == BID_RPAD)    return bn_rpad(args, nargs, out);
        if (_fb == BID_REMDR)   return bn_remdr(args, nargs, out);
        if (_fb == BID_DATE)    return bn_date(args, nargs, out);
        if (_fb == BID_TIME)    return bn_time(args, nargs, out);
        if (_fb == BID_IDENT)   return bn_identdiffer(args, nargs, out, 1);
        if (_fb == BID_DIFFER)  return bn_identdiffer(args, nargs, out, 0);
        if (_fb == BID_SNOx24NAME && nargs == 1) return bn_sno_name(args, nargs, out);
    }
    const size_t _fnlen = (bidlen >= 0) ? (size_t)((unsigned)bidlen >> 16) : strlen(fn);
    const int _bid = (bidlen >= 0) ? (int)(bidlen & 0xFFFF) : bid_of(fn, (unsigned)_fnlen);
    { extern long g_bidprof[1024]; extern int g_bidprof_on; extern void bidprof_init(void); if (g_bidprof_on < 0) bidprof_init(); if (g_bidprof_on && _bid >= 0 && _bid < 1024) g_bidprof[_bid]++; }
    dtax_ent_t *_dx = 0; int _dx_hit = 0; int _dx_skip_ctor = 0; int _dx_skip_syn = 0; unsigned _dxh = 5381u; unsigned char _dxl = 0; int _dx_bid_path = 0;
    if (!dtax_off()) { if (_bid > 0 && _bid <= 1024 && _fnlen && _fnlen < 14) { _dxl = (unsigned char)_fnlen; _dx = &g_dtax_bid[_bid]; _dx_bid_path = 1; } else { const char *_q = fn; while (*_q && _dxl < 14) { _dxh = _dxh * 131u + (unsigned char)*_q; _q++; _dxl++; } if (!(!*_q && _dxl)) _dxl = 0; else _dx = &g_dtax[_dxh & 255u]; }
      if (_dx) {
        if (_dx->gen == rt_dtax_gen && (_dx_bid_path || (_dx->len == _dxl && !memcmp(_dx->nm, fn, _dxl)))) {
          if (_dx->kind == 4 && _dx->ctor) return ((int (*)(DESCR_t *, int, DESCR_t *))_dx->ctor)(args, nargs, out);
          if (_dx->kind == 5 && _dx->ctor) return ((int (*)(DESCR_t *, int, DESCR_t *, int))_dx->ctor)(args, nargs, out, (int)_dx->nf);
          if (_dx->kind == 2 && _dx->syn) return try_call_builtin_by_name(_dx->syn, args, nargs, out);
          if (_dx->kind == 1 && _dx->ctor) { extern DESCR_t dat_construct_byref(void *, DESCR_t *, int);
            if (nargs <= (int)_dx->nf) { DESCR_t _fv[64]; int _nf = _dx->nf > 64 ? 64 : (int)_dx->nf;
              for (int _i = 0; _i < _nf; _i++) _fv[_i] = (_i < nargs) ? args[_i] : NULVCL;
              *out = dat_construct_byref(_dx->ctor, _fv, _nf); return 1; }
            _dx_hit = 1; _dx_skip_ctor = 1; }
          else if (_dx->kind == 0) { _dx_hit = 1; _dx_skip_ctor = 1; _dx_skip_syn = 1; }
          else if (_dx->kind == 3) { _dx_hit = 1; _dx_skip_ctor = 1; } } } }
    if (!_dx_skip_ctor) { extern DatType *dat_find_type(const char *); extern DESCR_t dat_construct(DatType *, DESCR_t *, int); extern int dat_nfields_byref(void *);
      DatType *_udt = dat_find_type(fn);
      if (_udt && _dx) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 1; _dx->nf = (short)dat_nfields_byref((void *)_udt); memcpy(_dx->nm, fn, _dxl); _dx->ctor = (void *)_udt; _dx->syn = 0; }
      if (_udt && nargs <= _udt->nfields) {
          DESCR_t _fv[64]; int _nf = _udt->nfields > 64 ? 64 : _udt->nfields;
          for (int _i = 0; _i < _nf; _i++) _fv[_i] = (_i < nargs) ? args[_i] : NULVCL;
          *out = dat_construct(_udt, _fv, _nf); return 1;
      }
      if (!_udt && _dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 3; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; } }
    { size_t _fl = _fnlen;
      if (_fl >= 2 && _fl <= 8) { int _r = -1;
        switch (((unsigned)_fl << 8) | (unsigned char)fn[0]) {
        case (2u<<8)|'E': if (fn[1]=='Q') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 0); _r = bn_numrel(args, nargs, out, 0); } break;
        case (2u<<8)|'N': if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 1); _r = bn_numrel(args, nargs, out, 1); } break;
        case (2u<<8)|'L': if (fn[1]=='T') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 2); _r = bn_numrel(args, nargs, out, 2); } else if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 3); _r = bn_numrel(args, nargs, out, 3); } break;
        case (2u<<8)|'G': if (fn[1]=='T') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 4); _r = bn_numrel(args, nargs, out, 4); } else if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 5); _r = bn_numrel(args, nargs, out, 5); } break;
        case (3u<<8)|'L': { int _o = fn[1]=='G'&&fn[2]=='T' ? 0 : fn[1]=='L'&&fn[2]=='T' ? 1 : fn[1]=='G'&&fn[2]=='E' ? 2 : fn[1]=='L'&&fn[2]=='E' ? 3 : fn[1]=='E'&&fn[2]=='Q' ? 4 : fn[1]=='N'&&fn[2]=='E' ? 5 : -1;
                            if (_o >= 0) { dtx5(_dx, _dxl, fn, (void *)bn_lexrel, (short)_o); _r = bn_lexrel(args, nargs, out, _o); } } break;
        case (4u<<8)|'S': if ((_bid == BID_SIZE)) { dtx4(_dx, _dxl, fn, (void *)bn_size); _r = bn_size(args, nargs, out); } break;
        case (4u<<8)|'T': if ((_bid == BID_TIME)) { dtx4(_dx, _dxl, fn, (void *)bn_time); _r = bn_time(args, nargs, out); } else if ((_bid == BID_TRIM)) { dtx4(_dx, _dxl, fn, (void *)bn_trim); _r = bn_trim(args, nargs, out); } break;
        case (4u<<8)|'D': if ((_bid == BID_DATE)) { dtx4(_dx, _dxl, fn, (void *)bn_date); _r = bn_date(args, nargs, out); } else if ((_bid == BID_DUPL)) { dtx4(_dx, _dxl, fn, (void *)bn_dupl); _r = bn_dupl(args, nargs, out); } break;
        case (4u<<8)|'F': if ((_bid == BID_FAIL)) { *out = FAILDESCR; return 1; } break;
        case (4u<<8)|'L': if ((_bid == BID_LPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_lpad); _r = bn_lpad(args, nargs, out); } break;
        case (4u<<8)|'R': if ((_bid == BID_RPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_rpad); _r = bn_rpad(args, nargs, out); } break;
        case (5u<<8)|'I': if ((_bid == BID_IDENT)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 1); _r = bn_identdiffer(args, nargs, out, 1); } break;
        case (5u<<8)|'R': if ((_bid == BID_REMDR)) { dtx4(_dx, _dxl, fn, (void *)bn_remdr); _r = bn_remdr(args, nargs, out); } break;
        case (6u<<8)|'S': if ((_bid == BID_SUBSTR)) { dtx4(_dx, _dxl, fn, (void *)bn_substr); _r = bn_substr(args, nargs, out); } break;
        case (6u<<8)|'D': if ((_bid == BID_DIFFER)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 0); _r = bn_identdiffer(args, nargs, out, 0); } break;
        case (7u<<8)|'R': if ((_bid == BID_REPLACE)) { dtx4(_dx, _dxl, fn, (void *)bn_replace); _r = bn_replace(args, nargs, out); } else if ((_bid == BID_REVERSE)) { dtx4(_dx, _dxl, fn, (void *)bn_reverse); _r = bn_reverse(args, nargs, out); } break;
        case (7u<<8)|'I': if ((_bid == BID_INTEGER)) { dtx4(_dx, _dxl, fn, (void *)bn_integer); _r = bn_integer(args, nargs, out); } break;
        case (7u<<8)|'S': if ((_bid == BID_SUCCEED)) { *out = NULVCL; return 1; } break;
        case (8u<<8)|'D': if ((_bid == BID_DATATYPE) && nargs == 1) _r = bn_type_datatype(fn, args, nargs, out); break;
        case (8u<<8)|'S': if ((_bid == BID_SNOx24NAME) && nargs == 1) _r = bn_sno_name(args, nargs, out); else if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; _r = 1; } break;
        default: break; }
        if (_r >= 0) return _r; } }
    if (icn_argtype_gate(_bid, args, nargs, out)) return 1;
    if (g_bidjmp_on) switch (_bid) { case BID___rk_bool: goto L_bidjmp_5076; case BID___rk_defined: goto L_bidjmp_5087; case BID___rk_dor: goto L_bidjmp_5090; case BID___rk_bool_val: goto L_bidjmp_5093; case BID___pas_chr: goto L_bidjmp_5103; case BID___pas_chrlit: goto L_bidjmp_5109; case BID___pas_enum_name: goto L_bidjmp_5112; case BID___pas_read_i: goto L_bidjmp_5121; case BID___pas_read_c: goto L_bidjmp_5124; case BID___pas_readln: goto L_bidjmp_5127; case BID___pas_eof: goto L_bidjmp_5130; case BID___pas_eoln: goto L_bidjmp_5135; case BID___pas_getbufch: goto L_bidjmp_5149; case BID___pas_trunc: goto L_bidjmp_5152; case BID___pas_abs: goto L_bidjmp_5156; case BID___pas_writeln: goto L_bidjmp_5171; case BID___pas_write: goto L_bidjmp_5171; case BID_write: goto L_bidjmp_5209; case BID_writes: goto L_bidjmp_5209; case BID_integer: goto L_bidjmp_5232; case BID_real: goto L_bidjmp_5264; case BID_string: goto L_bidjmp_5273; case BID_numeric: goto L_bidjmp_5283; case BID_cset: goto L_bidjmp_5320; case BID_ord: goto L_bidjmp_5330; case BID_image: goto L_bidjmp_5336; case BID_args: goto L_bidjmp_5339; case BID_proc: goto L_bidjmp_5385; case BID_repl: goto L_bidjmp_5524; case BID_reverse: goto L_bidjmp_5531; case BID_map: goto L_bidjmp_5537; case BID_trim: goto L_bidjmp_5567; case BID_getenv: goto L_bidjmp_5576; case BID_collect: goto L_bidjmp_5584; case BID_left: goto L_bidjmp_5585; case BID_right: goto L_bidjmp_5613; case BID_center: goto L_bidjmp_5639; case BID_detab: goto L_bidjmp_5671; case BID_entab: goto L_bidjmp_5704; case BID_abs: goto L_bidjmp_5747; case BID_max: goto L_bidjmp_5754; case BID_min: goto L_bidjmp_5765; case BID_sqrt: goto L_bidjmp_5777; case BID_atan: goto L_bidjmp_5791; case BID_log: goto L_bidjmp_5797; case BID_dtor: goto L_bidjmp_5803; case BID_rtod: goto L_bidjmp_5804; case BID_iand: goto L_bidjmp_5805; case BID_ior: goto L_bidjmp_5806; case BID_ixor: goto L_bidjmp_5807; case BID_ishift: goto L_bidjmp_5808; case BID_icom: goto L_bidjmp_5809; case BID_copy: goto L_bidjmp_5811; case BID_list: goto L_bidjmp_5840; case BID_table: goto L_bidjmp_5864; case BID_read: goto L_bidjmp_5874; case BID_reads: goto L_bidjmp_5883; case BID_runerr: goto L_bidjmp_5894; case BID_stop: goto L_bidjmp_5895; case BID_ICN_SCAN_PUSH: goto L_bidjmp_5900; case BID_ICN_SCAN_POP: goto L_bidjmp_5912; case BID_any: goto L_bidjmp_5920; case BID_many: goto L_bidjmp_5938; case BID_upto: goto L_bidjmp_5958; case BID_tab: goto L_bidjmp_5978; case BID_move: goto L_bidjmp_5991; case BID_pos: goto L_bidjmp_6004; case BID_match: goto L_bidjmp_6012; case BID_bal: goto L_bidjmp_6033; case BID_find: goto L_bidjmp_6067; case BID_NONNULL: goto L_bidjmp_6081; case BID_ICN_CASE_EQ: goto L_bidjmp_6088; case BID_ICN_SWAP_TOP2: goto L_bidjmp_6104; case BID_ICN_NULL: goto L_bidjmp_6108; case BID_insert: goto L_bidjmp_6115; case BID_delete: goto L_bidjmp_6124; case BID_member: goto L_bidjmp_6132; case BID_key: goto L_bidjmp_6140; case BID___apply__: goto L_bidjmp_6149; case BID_push: goto L_bidjmp_6162; case BID_put: goto L_bidjmp_6182; case BID_get: goto L_bidjmp_6206; case BID_pop: goto L_bidjmp_6221; case BID_pull: goto L_bidjmp_6236; case BID_sort: goto L_bidjmp_6249; case BID_FIELD_GET: goto L_bidjmp_6322; case BID_FIELD_SET: goto L_bidjmp_6330; case BID_MAKELIST: goto L_bidjmp_6341; case BID_RECORD_REGISTER: goto L_bidjmp_6345; case BID_RECORD_MAKE: goto L_bidjmp_6351; case BID_open: goto L_bidjmp_6361; case BID_remove: goto L_bidjmp_6376; case BID_close: goto L_bidjmp_6381; case BID_IDENTICAL: goto L_bidjmp_6411; case BID_set: goto L_bidjmp_6423; case BID_ASGN: goto L_bidjmp_6439; case BID_name: goto L_bidjmp_6446; case BID_variable: goto L_bidjmp_6462; case BID_SNOx24NAME: goto L_bidjmp_6468; case BID_ARRAY: goto L_bidjmp_6469; case BID_TABLE: goto L_bidjmp_6479; case BID_ITEM: goto L_bidjmp_6484; case BID_PROTOTYPE: goto L_bidjmp_6490; case BID_CONVERT: goto L_bidjmp_6499; case BID_DATA: goto L_bidjmp_6521; case BID_SNOx24KWSET: goto L_bidjmp_6528; case BID_SNOx24STMT: goto L_bidjmp_6534; case BID_SNOx24MKEXPR: goto L_bidjmp_6540; case BID_SNOx24PBK: goto L_bidjmp_6545; case BID_SNOx24PBN: goto L_bidjmp_6546; case BID_SNOx24PB0: goto L_bidjmp_6547; case BID_SNOx24PBC: goto L_bidjmp_6548; case BID_SNOx24PCUR: goto L_bidjmp_6549; case BID_SNOx24PBALT: goto L_bidjmp_6550; case BID_SNOx24PARB: goto L_bidjmp_6551; case BID_SNOx24PFEN: goto L_bidjmp_6552; case BID_SNOx24PDEF: goto L_bidjmp_6553; case BID_SNOx24MKPAT: goto L_bidjmp_6554; case BID_OPSYN: goto L_bidjmp_6563; case BID_CODE: goto L_bidjmp_6567; case BID_EVAL: goto L_bidjmp_6572; case BID_VALUE: goto L_bidjmp_6581; case BID_SNOx24NRET: goto L_bidjmp_6587; case BID_SNOx24WANTNM: goto L_bidjmp_6588; case BID_APPLY: goto L_bidjmp_6589; default: break; }
    L_bidjmp_5076: ;
    if ((_bid == BID___rk_bool) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v))  t = 0;
        else if (IS_INT_fn(v))  t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else { const char *s = v.s ? v.s : ""; t = (s[0] != '\0' && !(s[0]=='0' && s[1]=='\0')); }
        if (!t) { *out = FAILDESCR; return 1; }
        *out = v; return 1;
    }
    L_bidjmp_5087: ;
    if ((_bid == BID___rk_defined) && nargs == 1) {
        DESCR_t v = args[0]; int t = (!IS_FAIL_fn(v)) && (v.v != DT_SNUL); *out = t ? INTVAL(1) : FAILDESCR; return 1;
    }
    L_bidjmp_5090: ;
    if ((_bid == BID___rk_dor) && nargs == 2) {
        DESCR_t a = args[0]; int def = (!IS_FAIL_fn(a)) && (a.v != DT_SNUL); *out = def ? a : args[1]; return 1;
    }
    L_bidjmp_5093: ;
    if ((_bid == BID___rk_bool_val) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v))  t = 0;
        else if (IS_INT_fn(v))  t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else { const char *s = v.s ? v.s : ""; t = (s[0] != '\0' && !(s[0]=='0' && s[1]=='\0')); }
        *out = INTVAL(t); return 1;
    }
    L_bidjmp_5103: ;
    if ((_bid == BID___pas_chr) && nargs == 1) {
        long long cv = IS_INT_fn(args[0]) ? args[0].i : 0;
        if (cv < 0) cv = 0; if (cv > 255) cv = 255;
        char *s = (char *)rt_ws_alloc(2); s[0] = (char)(unsigned char)cv; s[1] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    L_bidjmp_5109: ;
    if ((_bid == BID___pas_chrlit) && nargs == 1) {
        *out = IS_INT_fn(args[0]) ? args[0] : INTVAL(0); return 1;
    }
    L_bidjmp_5112: ;
    if ((_bid == BID___pas_enum_name) && nargs == 2) {
        long long ord = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        const char *csv = VARVAL_fn(args[1]); if (!csv) csv = "";
        const char *p = csv; long long k = 0;
        while (*p && k < ord) { if (*p == ',') k++; p++; }
        const char *st = p; while (*p && *p != ',') p++;
        size_t L = (size_t)(p - st); char *s = (char *)rt_ws_alloc(L + 1); memcpy(s, st, L); s[L] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    L_bidjmp_5121: ;
    if ((_bid == BID___pas_read_i) && nargs == 0) {
        long long v = 0; scanf(" %lld", &v); *out = INTVAL(v); return 1;
    }
    L_bidjmp_5124: ;
    if ((_bid == BID___pas_read_c) && nargs == 0) {
        int c = getchar(); if (c == EOF) c = 26; *out = INTVAL((long long)(unsigned char)c); return 1;
    }
    L_bidjmp_5127: ;
    if ((_bid == BID___pas_readln) && nargs == 0) {
        int c; while ((c = getchar()) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1;
    }
    L_bidjmp_5130: ;
    if ((_bid == BID___pas_eof) && nargs == 0) {
        int c = getchar();
        if (c == EOF) { *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    L_bidjmp_5135: ;
    if ((_bid == BID___pas_eoln) && nargs == 0) {
        int c = getchar();
        if (c == EOF || c == '\n') { if (c != EOF) ungetc(c, stdin); *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    if (((_bid == BID___pas_read_i_f) || (_bid == BID___pas_read_c_f) || (_bid == BID___pas_readln_f) || (_bid == BID___pas_eof_f) || (_bid == BID___pas_eoln_f) || (_bid == BID___pas_getbufch_f)) && nargs == 1) {
        extern FILE *fh_get(int); FILE *f = IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : NULL; if (!f) f = stdin;
        if ((_bid == BID___pas_read_i_f)) { long long v = 0; if (fscanf(f, " %lld", &v) != 1) v = 0; *out = INTVAL(v); return 1; }
        if ((_bid == BID___pas_read_c_f)) { int c = fgetc(f); if (c == EOF) c = 26; *out = INTVAL((long long)(unsigned char)c); return 1; }
        if ((_bid == BID___pas_readln_f)) { int c; while ((c = fgetc(f)) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1; }
        if ((_bid == BID___pas_eof_f)) { int c = fgetc(f); if (c == EOF) { *out = INTVAL(1); return 1; } ungetc(c, f); *out = INTVAL(0); return 1; }
        if ((_bid == BID___pas_eoln_f)) { int c = fgetc(f); if (c == EOF || c == '\n') { if (c != EOF) ungetc(c, f); *out = INTVAL(1); return 1; } ungetc(c, f); *out = INTVAL(0); return 1; }
        { int c = fgetc(f); if (c == EOF) { *out = INTVAL((long long)' '); return 1; } ungetc(c, f); *out = INTVAL((long long)(unsigned char)c); return 1; }
    }
    L_bidjmp_5149: ;
    if ((_bid == BID___pas_getbufch) && nargs == 0) {
        int c = getchar(); if (c == EOF) { *out = INTVAL((long long)' '); return 1; } ungetc(c, stdin); *out = INTVAL((long long)(unsigned char)c); return 1;
    }
    L_bidjmp_5152: ;
    if ((_bid == BID___pas_trunc) && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = INTVAL((long long)d); return 1;
    }
    L_bidjmp_5156: ;
    if ((_bid == BID___pas_abs) && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d < 0 ? -d : d; *out = r; return 1; }
        long long v = IS_INT_fn(args[0]) ? args[0].i : 0; *out = INTVAL(v < 0 ? -v : v); return 1;
    }
    if (nargs == 1 && ((_bid == BID___pas_sin)||(_bid == BID___pas_cos)||(_bid == BID___pas_exp)||(_bid == BID___pas_sqrt)||(_bid == BID___pas_ln)||(_bid == BID___pas_arctan))) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        double r;
        if      ((_bid == BID___pas_sin))    r = sin(d);
        else if ((_bid == BID___pas_cos))    r = cos(d);
        else if ((_bid == BID___pas_exp))    r = exp(d);
        else if ((_bid == BID___pas_sqrt))   r = sqrt(d);
        else if ((_bid == BID___pas_ln))     r = log(d);
        else                                 r = atan(d);
        DESCR_t rv; rv.v = DT_R; rv.r = r; *out = rv; return 1;
    }
    if ((_bid == BID___pas_round) && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = INTVAL((long long)round(d)); return 1;
    }
    if ((_bid == BID___pas_halt) && (nargs == 0 || nargs == 1)) {
        fflush(NULL);
        exit(nargs == 1 ? (int)(IS_INT_fn(args[0]) ? args[0].i : 0) : 0);
    }
    if ((_bid == BID___pas_frac) && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        DESCR_t r; r.v = DT_R; r.r = d - trunc(d); *out = r; return 1;
    }
    if ((_bid == BID___pas_assert) && nargs == 1) {
        long long cond = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (args[0].r != 0.0) : 0);
        if (!cond) { fflush(NULL); fprintf(stderr, "Runtime error 227 at $0\n"); exit(227); }
        *out = NULVCL; return 1;
    }
    if ((_bid == BID___pas_mkdir) && nargs == 1) {
        char sb[256]; const char *d = to_cstring(args[0], sb, sizeof sb);
        mkdir(d, 0777); *out = NULVCL; return 1;
    }
    if ((_bid == BID___pas_rmdir) && nargs == 1) {
        char sb[256]; const char *d = to_cstring(args[0], sb, sizeof sb);
        rmdir(d); *out = NULVCL; return 1;
    }
    if ((_bid == BID___pas_pos) && nargs == 2) {
        char nb[64]; const char *needle = to_cstring(args[0], nb, sizeof nb);
        char hb[64]; const char *hay = to_cstring(args[1], hb, sizeof hb);
        const char *hit = (needle && needle[0]) ? strstr(hay, needle) : NULL;
        *out = INTVAL(hit ? (long long)(hit - hay) + 1 : 0); return 1;
    }
    L_bidjmp_5171: ;
    if ((_bid == BID___pas_writeln) || (_bid == BID___pas_write)) {
        int nl = (fn[6] == 'w' && fn[7] == 'r' && fn[8] == 'i' && fn[9] == 't' && fn[10] == 'e' && fn[11] == 'l');
        int _start = 0;
        FILE *_dest = stdout;
        if (nargs >= 1 && IS_FH_fn(args[0])) { extern FILE *fh_get(int); FILE *_fp = fh_get((int)args[0].i); if (_fp) _dest = _fp; _start = 2; }
        for (int _pi = _start; _pi + 1 < nargs; _pi += 2) {
            DESCR_t av = args[_pi];
            DESCR_t aw = args[_pi + 1];
            int w = IS_INT_fn(aw) ? (aw.i == -3 ? -3 : (aw.i >= 0 ? (int)aw.i : -1)) : -1;
            if (w == -3) { double _rv = IS_REAL_fn(av) ? av.r : (IS_INT_fn(av) ? (double)av.i : 0.0);
                long _fw2 = 0, _fp2 = 0;
                if (_pi + 3 < nargs) { if (IS_INT_fn(args[_pi+2])) _fw2 = (long)args[_pi+2].i; if (IS_INT_fn(args[_pi+3])) _fp2 = (long)args[_pi+3].i; }
                if (_fw2 < 0) _fw2 = 0; if (_fp2 < 0) _fp2 = 0; if (_fp2 > 16) _fp2 = 16;
                fprintf(_dest, "%*.*f", (int)_fw2, (int)_fp2, _rv); _pi += 2; continue; }
            if (IS_INT_fn(av)) {
                char _pb[32];
                int _pfmtlen = snprintf(_pb, sizeof _pb, "%lld", (long long)av.i);
                int _fw = (w < 0) ? 11 : (w > _pfmtlen ? w : _pfmtlen);
                fprintf(_dest, "%*s", _fw, _pb);
            } else if (IS_REAL_fn(av)) {
                char _rb[64];
                int _prec = (w < 0) ? 12 : (w - 8 < 1 ? 1 : (w - 8 > 16 ? 16 : w - 8));
                pas_real_str(av.r, _rb, sizeof _rb, _prec);
                int _pfmtlen = (int)strlen(_rb);
                int _fw = (w < 0) ? 20 : (_pfmtlen + 1 > w ? _pfmtlen + 1 : w);
                fprintf(_dest, "%*s", _fw, _rb);
            } else {
                const char *_ps = VARVAL_fn(av);
                if (_ps) {
                    if (w == -2) { fprintf(_dest, "%c", (int)(unsigned char)_ps[0]); }
                    else if (w >= 0) { fprintf(_dest, "%*.*s", w, w, _ps); }
                    else { fputs(_ps, _dest); }
                }
            }
        }
        if (nl) fputc('\n', _dest);
        *out = NULVCL; return 1;
    }
    L_bidjmp_5209: ;
    if ((_bid == BID_write) || (_bid == BID_writes)) {
        { extern int g_icon_write_reassignable; if (!g_icon_write_reassignable) {  goto L_write_body_5209; } }
        { DESCR_t _wv = NV_GET_fn(fn);
          int _is_self_default = (_wv.v == DT_E && _wv.slen == 0xFFFFFFFEu && _wv.s && !strcmp(_wv.s, fn));
          if (!_is_self_default) { *out = rt_call_value(_wv, args, nargs); return 1; } }
        L_write_body_5209: ;
        int nl = (fn[5] == '\0');
        int start = 0;
        FILE *dest = stdout;
        if (nargs > 0 && IS_FH_fn(args[0])) {
            FILE *fp = fh_get((int)args[0].i);
            if (fp) { if (nl && fp != stdout) fflush(stdout); dest = fp; }
            start = 1;
        }
        for (int _wi = start; _wi < nargs; _wi++) {
            DESCR_t av = args[_wi];
            if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
            if (av.v == DT_SNUL) continue;
            if (dest != stdout && dest != stderr && IS_STR_fn(av)) { const char *_bs = VARVAL_fn(av); uint32_t _bn = av.slen ? av.slen : (_bs ? (uint32_t)strlen(_bs) : 0u); if (_bs && _bn) fwrite(_bs, 1, _bn, dest); continue; }
            out_write_descr(dest, av, nl);
        }
        if (nl) fputc('\n', dest);
        *out = nargs > start ? args[nargs-1] : (nargs > 0 ? args[0] : NULVCL);
        return 1;
    }
    L_bidjmp_5232: ;
    if ((_bid == BID_integer) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av))  { *out = av; return 1; }
        if (IS_REAL_fn(av)) { *out = INTVAL((long long)av.r); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
        { long long rv; if (icon_radix_int(s, &rv)) { *out = INTVAL(rv); return 1; } }
        char *end; long long iv = strtoll(s, &end, 10);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL(iv); return 1; }
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL((long long)rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5264: ;
    if ((_bid == BID_real) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_REAL_fn(av)) { *out = av; return 1; }
        if (IS_INT_fn(av))  { *out = REALVAL((double)av.i); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
        char *end; double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = REALVAL(rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5273: ;
    if ((_bid == BID_string) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = rt_str_coerce(av); return 1; }
        if (IS_STR_fn(av)) { *out = av; return 1; }
        if (av.v == DT_BIG) { extern char *rt_big_str(DESCR_t); *out = STRVAL(rt_big_str(av)); return 1; }
        char *buf = rt_ws_alloc(64);
        if (IS_INT_fn(av))       snprintf(buf,64,"%lld",(long long)av.i);
        else if (IS_REAL_fn(av)) { icon_real_str(av.r,buf,64); }
        else { *out = NULVCL; return 1; }
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5283: ;
    if ((_bid == BID_numeric) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av)||IS_REAL_fn(av)) { *out = av; return 1; }
        const char *s = VARVAL_fn(av); if (!s||!*s) { *out = FAILDESCR; return 1; }
        {
            const char *p = s;
            while (*p == ' ' || *p == '\t') p++;
            int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
            int base = 0; const char *bstart = p;
            while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
            if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                p++; const char *dstart = p; long long v = 0;
                while (*p) {
                    int d = -1;
                    if (*p >= '0' && *p <= '9') d = *p - '0';
                    else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                    else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                    if (d < 0 || d >= base) break;
                    v = v * base + d;
                    p++;
                }
                while (*p == ' ' || *p == '\t') p++;
                if (p > dstart && *p == '\0') { *out = INTVAL(neg ? -v : v); return 1; }
            }
        }
        char *end; long long iv = strtoll(s, &end, 10);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL(iv); return 1; }
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = REALVAL(rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (((_bid == BID_char) || (_bid == BID_chr)) && nargs == 1) {
        DESCR_t av = args[0];
        const char *raw = VARVAL_fn(av); long long rv;
        int n = (int)(IS_INT_fn(av) ? av.i : (raw && icon_radix_int(raw, &rv)) ? rv : (long long)strtol(raw?raw:"0",NULL,10));
        char *buf = rt_ws_alloc(2); buf[0]=(char)(n&0xFF); buf[1]='\0';
        *out = BSTRVAL(buf, 1); return 1;
    }
    L_bidjmp_5320: ;
    if ((_bid == BID_cset) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = av; return 1; }
        char _cbuf[64];
        const char *raw; int rawlen;
        if (IS_INT_fn(av))       { snprintf(_cbuf,sizeof _cbuf,"%lld",(long long)av.i); raw=_cbuf; rawlen=(int)strlen(_cbuf); }
        else if (IS_REAL_fn(av)) { icon_real_str(av.r,_cbuf,sizeof _cbuf); raw=_cbuf; rawlen=(int)strlen(_cbuf); }
        else { raw = VARVAL_fn(av); if (!raw) raw = ""; rawlen=(int)descr_slen(av); }
        *out = CSETVAL(cset_canonical(raw, rawlen)); return 1;
    }
    L_bidjmp_5330: ;
    if ((_bid == BID_ord) && nargs == 1) {
        DESCR_t av = args[0];
        const char *s = VARVAL_fn(av);
        if (!s || !descr_slen(av)) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]); return 1;
    }
    if (((_bid == BID_type) || (_bid == BID_DATATYPE)) && nargs == 1) return bn_type_datatype(fn, args, nargs, out);
    L_bidjmp_5336: ;
    if ((_bid == BID_image) && nargs == 0) {
        *out = STRVAL("&null"); return 1;
    }
    L_bidjmp_5339: ;
    if ((_bid == BID_args) && nargs == 1) {
        DESCR_t a = args[0];
        if (a.v == DT_E && a.slen == 0xFFFFFFFEu) {
            static const struct { const char *nm; int np; } _bt[] = {
                {"push",-2},{"put",-2},{"insert",-2},{"delete",-2},
                {"pop",1},{"get",1},{"pull",1},{"bal",3},{"find",3},{"upto",3},
                {"any",3},{"many",3},{"match",3},{"tab",1},{"move",1},{"pos",1},
                {"write",-1},{"writes",-1},{"writef",-1},
                {"read",0},{"reads",1},{"close",1},{"open",2},{"flush",1},
                {"image",1},{"type",1},{"copy",1},{"sort",2},{"sortf",2},
                {"reverse",1},{"size",1},{"left",3},{"right",3},{"center",3},
                {"string",1},{"integer",1},{"real",1},{"char",1},{"ord",1},
                {"trim",2},{"map",3},{"repl",2},{"cset",1},{"list",2},
                {"table",1},{"set",1},{"key",1},{"member",2},
                {"proc",2},{"args",1},{"name",1},{"variable",1},
                {"abs",1},{"sqrt",1},{"sin",1},{"cos",1},{"atan",2},
                {"exp",1},{"log",2},{"max",-1},{"min",-1},
                {"iand",2},{"ior",2},{"ixor",2},{"icom",1},{"ishift",2},
                {"numeric",1},{"entab",-1},{"detab",-1},{"seq",2},
                {"collect",2},{"display",2},{"runerr",2},{"errorclear",0},
                {"function",0},{"serial",1},{"system",1},{"exit",1},
                {NULL,0}};
            int np = rt_proc_nparams(a.s);
            if (np < 0 && a.s) for (int _bi=0;_bt[_bi].nm;_bi++) if (!strcmp(_bt[_bi].nm,a.s)){np=_bt[_bi].np;break;}
            if (np < 0 && a.s && !strcmp(a.s, "main")) {
                np = 0;
                for (int i = 0; i < g_stage2.proc_count; i++)
                    if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, "main")) { np = g_stage2.proc_table[i].nparams; break; }
            }
            if (np >= 0) { *out = INTVAL(np); return 1; }
            *out = INTVAL(np < -1 ? np : -1); return 1;
        }
        if (a.v == DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++) {
                if (g_stage2.proc_table[i].entry_pc == (int)a.i) {
                    *out = INTVAL(g_stage2.proc_table[i].nparams <= 0 ? -2 : g_stage2.proc_table[i].nparams);
                    return 1;
                }
            }
            *out = INTVAL(-2); return 1;
        }
        if (IS_STR_fn(a)) {
            *out = INTVAL(-2); return 1;
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5385: ;
    if ((_bid == BID_proc) && (nargs == 2 || nargs == 1)) {
        const char *pname = VARVAL_fn(args[0]);
        int arity = (nargs >= 2) ? (int)to_int(args[1]) : -1;
        if (!pname) { *out = FAILDESCR; return 1; }
        for (int i = 0; i < g_stage2.proc_count; i++) {
            if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, pname) == 0) {
                if (arity < 0 || g_stage2.proc_table[i].nparams == arity || g_stage2.proc_table[i].nparams <= 0) {
                    extern DESCR_t rt_proc_value(const char *);
                    *out = rt_proc_value(g_stage2.proc_table[i].name); return 1;
                }
            }
        }
        { extern int rt_proc_is_registered(const char *name); extern int rt_proc_nparams(const char *name);
          if (rt_proc_is_registered(pname)) { int np = rt_proc_nparams(pname);
              if (arity < 0 || np == arity || np <= 0) { extern DESCR_t rt_proc_value(const char *); *out = rt_proc_value(rt_ws_strdup(pname)); return 1; } } }
        if (icn_builtin_is_known(pname) || rt_builtin_is_known(pname)) {
            DESCR_t bv; bv.v = DT_E; bv.slen = 0xFFFFFFFEu; bv.s = rt_ws_strdup(pname); *out = bv; return 1;
        }
        { static const char *const op2[] = { "+","-","*","/","%","^","||","|||","++","--","**","<","<=",">",">=","=","~=","<<","<<=",">>",">>=","==","~==","===","~===", 0 };
          static const char *const op1[] = { "+","-","*","/","\\","=","?","~","!","@","^", 0 };
          const char **tbl = (arity == 2 || arity == 3) ? op2 : (arity == 1 || arity < 0) ? op1 : 0;
          if (tbl) for (int oi = 0; tbl[oi]; oi++) if (!strcmp(tbl[oi], pname)) { DESCR_t bv; bv.v = DT_E; bv.slen = 0xFFFFFFFEu; bv.s = rt_ws_strdup(pname); *out = bv; return 1; } }
        *out = FAILDESCR; return 1;
    }
    if ((_bid == BID_image) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(256);
        if (av.v == DT_SNUL)     { *out = STRVAL("&null"); return 1; }
        if (av.v == DT_E) {
            const char *nm = procval_name(av);
            if (!nm) nm = "?";
            snprintf(buf,256, dat_find_type(nm) ? "record constructor %s" : (rt_proc_is_registered(nm) || !strcmp(nm, "main")) ? "procedure %s" : "function %s", nm);
            *out = STRVAL(buf); return 1;
        }
        if (IS_FH_fn(av)) {
            int idx = (int)av.i;
            if (idx == 0) { snprintf(buf,256,"&input");  *out = STRVAL(buf); return 1; }
            if (idx == 1) { snprintf(buf,256,"&output"); *out = STRVAL(buf); return 1; }
            if (idx == 2) { snprintf(buf,256,"&errout"); *out = STRVAL(buf); return 1; }
            if (idx >= 0 && idx < FH_MAX && fh_name[idx]) {
                snprintf(buf,256,"file(%s)",fh_name[idx]);
                *out = STRVAL(buf); return 1;
            }
            snprintf(buf,256,"file(?)"); *out = STRVAL(buf); return 1;
        }
        if (IS_INT_fn(av)) {
            snprintf(buf,256,"%lld",(long long)av.i); *out = STRVAL(buf); return 1;
        }
        if (IS_REAL_fn(av))      { icon_real_str(av.r,buf,128); *out = STRVAL(buf); return 1; }
        if (av.v==DT_T)          { snprintf(buf,128,"%s_%ld(%d)", (av.tbl && av.tbl->is_set) ? "set" : "table", av.tbl?av.tbl->id:0, av.tbl?av.tbl->size:0); *out = STRVAL(buf); return 1; }
        if (av.v==DT_DATA && av.u) {
            const char *tname = av.u->type ? av.u->type->name : "record";
            if (strcmp(tname,"list")==0) {
                int cnt = (av.u->type && av.u->type->nfields>=2 && av.u->fields)
                          ? (int)av.u->fields[1].i : 0;
                long id = rt_record_image_id(av.u);
                snprintf(buf,128,"list_%ld(%d)",id,cnt); *out = STRVAL(buf); return 1;
            }
            { int nf = (av.u->type ? av.u->type->nfields : 0); long id = rt_record_image_id(av.u); snprintf(buf,256,"record %s_%ld(%d)",tname,id,nf); *out = STRVAL(buf); return 1; }
        }
        if (av.v==DT_DATA)       { *out = STRVAL("record"); return 1; }
        if (av.v==DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++)
                if (g_stage2.proc_table[i].entry_pc==(int)av.i)
                    { snprintf(buf,128,"procedure %s",g_stage2.proc_table[i].name); *out=STRVAL(buf); return 1; }
            snprintf(buf,128,"procedure"); *out=STRVAL(buf); return 1;
        }
        if (IS_CSET_fn(av)) {
            const char *cs = av.s ? av.s : "";
            const char *kname = kw_cset_name(cs);
            if (kname) { *out = STRVAL(kname); return 1; }
            int _kl = kw_cset_len(cs);
            int cslen = (_kl >= 0) ? _kl : (int)strlen(cs);
            char *outs = rt_ws_alloc(cslen * 4 + 3);
            int o = 0;
            outs[o++] = '\'';
            for (int i = 0; i < cslen; i++) {
                unsigned char c = (unsigned char)cs[i];
                if (c == '\'') { outs[o++] = '\\'; outs[o++] = '\''; }
                else if (c < 0x20 || c >= 0x7f) { o += snprintf(outs+o, 5, "\\x%02x", c); }
                else outs[o++] = (char)c;
            }
            outs[o++] = '\'';
            outs[o] = '\0';
            *out = STRVAL(outs); return 1;
        }
        if (IS_STR_fn(av) && av.s) {
        }
        const char *s=VARVAL_fn(av); if (!s) s = "";
        int sl = (av.slen && av.slen != 0xFFFFFFFFu) ? (int)av.slen : (int)strlen(s);
        char *outs = rt_ws_alloc(sl*4 + 3);
        int o = 0;
        outs[o++] = '"';
        for (int i = 0; i < sl; i++) {
            unsigned char c = (unsigned char)s[i];
            switch (c) {
                case '"':  outs[o++]='\\'; outs[o++]='"';  break;
                case '\\': outs[o++]='\\'; outs[o++]='\\'; break;
                case '\n': outs[o++]='\\'; outs[o++]='n';  break;
                case '\t': outs[o++]='\\'; outs[o++]='t';  break;
                case '\r': outs[o++]='\\'; outs[o++]='r';  break;
                case '\b': outs[o++]='\\'; outs[o++]='b';  break;
                case '\v': outs[o++]='\\'; outs[o++]='v';  break;
                case '\f': outs[o++]='\\'; outs[o++]='f';  break;
                case 0x1b: outs[o++]='\\'; outs[o++]='e';  break;
                case 0x7f: outs[o++]='\\'; outs[o++]='d';  break;
                default:
                    if (c < 0x20 || c >= 0x7f) {
                        o += snprintf(outs+o, 5, "\\x%02x", c);
                    } else {
                        outs[o++] = (char)c;
                    }
            }
        }
        outs[o++] = '"';
        outs[o] = '\0';
        *out = STRVAL(outs); return 1;
    }
    if ((_bid == BID_image) && nargs >= 2) {
        DESCR_t av = args[0];
        if (0 && IS_STR_fn(av) && av.s) { }
        DESCR_t one_out = FAILDESCR;
        if (try_call_builtin_by_name("image", args, 1, &one_out))
            { *out = one_out; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5524: ;
    if ((_bid == BID_repl) && nargs == 2) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int n=(int)to_int(args[1]); if(n<0)n=0;
        int sl=icn_true_len(args[0], s); char *buf=rt_ws_alloc(sl*n+1); buf[0]='\0';
        for(int i=0;i<n;i++) memcpy(buf+i*sl,s,sl); buf[sl*n]='\0';
        *out = BSTRVAL(buf, sl*n); return 1;
    }
    L_bidjmp_5531: ;
    if ((_bid == BID_reverse) && nargs == 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=icn_true_len(args[0], s); char *buf=rt_ws_alloc(sl+1);
        for(int i=0;i<sl;i++) buf[i]=s[sl-1-i]; buf[sl]='\0';
        *out = BSTRVAL(buf, sl); return 1;
    }
    L_bidjmp_5537: ;
    if ((_bid == BID_map) && nargs >= 1 && nargs <= 3) {
        static const char *const UCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const char *const LCASE = "abcdefghijklmnopqrstuvwxyz";
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        const char *from = UCASE, *to = LCASE;
        if (nargs >= 2) {
            DESCR_t fv=args[1];
            if (fv.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fv);
                if (fs) from = fs;
            }
        }
        if (nargs >= 3) {
            DESCR_t tv=args[2];
            if (tv.v != DT_SNUL) {
                const char *ts = VARVAL_fn(tv);
                if (ts) to = ts;
            }
        }
        int sl=icn_true_len(args[0], s); char *buf=rt_ws_alloc(sl+1);
        int fl=icn_true_len(nargs >= 2 ? args[1] : NULVCL, from), tl=icn_true_len(nargs >= 3 ? args[2] : NULVCL, to);
        for (int i=0;i<sl;i++) {
            char c=s[i]; int hit=0;
            for (int j=fl-1;j>=0;j--) {
                if (from[j]==c) { buf[i] = (j<tl) ? to[j] : c; hit=1; break; }
            }
            if (!hit) buf[i]=c;
        }
        buf[sl]='\0'; *out = BSTRVAL(buf, sl); return 1;
    }
    L_bidjmp_5567: ;
    if ((_bid == BID_trim) && (nargs == 1 || nargs == 2)) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        const char *cset = " ";
        if (nargs == 2) { DESCR_t cv = args[1]; if (cv.v != DT_SNUL) { const char *cs = VARVAL_fn(cv); if (cs) cset = cs; } }
        int sl=icn_true_len(args[0], s);
        while (sl > 0 && strchr(cset, s[sl-1])) sl--;
        char *buf=rt_ws_alloc(sl+1); memcpy(buf,s,sl); buf[sl]='\0';
        *out = BSTRVAL(buf, sl); return 1;
    }
    L_bidjmp_5576: ;
    if ((_bid == BID_getenv) && nargs == 1) {
        const char *name = VARVAL_fn(args[0]);
        if (!name) { *out = FAILDESCR; return 1; }
        const char *val = getenv(name);
        if (!val) { *out = FAILDESCR; return 1; }
        size_t vl = strlen(val); char *buf = rt_ws_alloc(vl+1); memcpy(buf, val, vl); buf[vl] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5584: ;
    if ((_bid == BID_collect) && nargs <= 2) { extern long rt_gc_collect(void); rt_gc_collect(); *out = NULVCL; return 1; }
    L_bidjmp_5585: ;
    if ((_bid == BID_left) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=icn_true_len(args[0], s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int copy = sl < n ? sl : n;
        for (int i = 0; i < copy; i++) buf[i] = s[i];
        int rpad = n - copy;
        for (int k = 0; k < rpad; k++) {
            int idx = ((k + fl - rpad) % fl + fl) % fl;
            buf[copy + k] = fill[idx];
        }
        buf[n]='\0';
        *out = BSTRVAL(buf, n); return 1;
    }
    L_bidjmp_5613: ;
    if ((_bid == BID_right) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=icn_true_len(args[0], s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int pad = n - sl; if (pad < 0) pad = 0;
        for (int i = 0; i < pad; i++) buf[i] = fill[i % fl];
        int srcoff = (sl > n) ? (sl - n) : 0;
        int copy = sl - srcoff; if (pad + copy > n) copy = n - pad;
        for (int i = 0; i < copy; i++) buf[pad + i] = s[srcoff + i];
        buf[n]='\0';
        *out = BSTRVAL(buf, n); return 1;
    }
    L_bidjmp_5639: ;
    if ((_bid == BID_center) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=icn_true_len(args[0], s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int lpad = (n - sl) / 2; if (lpad < 0) lpad = 0;
        int srcoff = (sl > n) ? (sl - n + 1) / 2 : 0;
        int copy = sl - srcoff; if (lpad + copy > n) copy = n - lpad;
        int rpad = n - lpad - copy;
        for (int i = 0; i < lpad; i++) buf[i] = fill[i % fl];
        for (int i = 0; i < copy; i++) buf[lpad + i] = s[srcoff + i];
        for (int k = 0; k < rpad; k++) {
            int idx = ((k + fl - rpad) % fl + fl) % fl;
            buf[lpad + copy + k] = fill[idx];
        }
        buf[n]='\0';
        *out = BSTRVAL(buf, n); return 1;
    }
    if (((_bid == BID_detab) || (_bid == BID_entab)) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    L_bidjmp_5671: ;
    if ((_bid == BID_detab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_detab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) { const char *_p = args[j].s; while (*_p == ' ') _p++; if (*_p == '+' || *_p == '-') _p++; int _d = 0; while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; } while (*_p == ' ') _p++; _cok = _d && !*_p; }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = rt_ws_alloc(cap); int bi = 0, col = 0; int slen0 = icn_true_len(args[0], s);
        for (int i = 0; i < slen0; i++) {
            if (s[i] == '\t') {
                int next = -1;
                for (int k = 0; k < nstops; k++) if (stops[k] > col+1) { next=stops[k]; break; }
                if (next < 0) {
                    int base = stops[nstops-1];
                    int beyond = col + 1 - base;
                    next = base + ((beyond / gap) + 1) * gap;
                }
                int sp = next - (col+1);
                while (sp-- > 0) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; col++; }
            } else if (s[i] == '\b') {
                if (col > 0) col--;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
            } else if (s[i] == '\n' || s[i] == '\r') {
                col = 0;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
            } else {
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
                if (isprint((unsigned char)s[i])) col++;
            }
        }
        buf[bi]='\0'; *out=BSTRVAL(buf, bi); return 1;
    }
    L_bidjmp_5704: ;
    if ((_bid == BID_entab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_entab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) { const char *_p = args[j].s; while (*_p == ' ') _p++; if (*_p == '+' || *_p == '-') _p++; int _d = 0; while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; } while (*_p == ' ') _p++; _cok = _d && !*_p; }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = rt_ws_alloc(cap); int bi = 0, col = 1;
        int slen = icn_true_len(args[0], s);
        for (int i = 0; i < slen; ) {
            char c = s[i];
            if (c == ' ') {
                int target = col + 1; int j = i + 1;
                while (j < slen && s[j] == ' ') { target++; j++; }
                if (target - col > 1) {
                    int nt = icn_nxttab(col, stops, nstops, gap);
                    int do_tab = 1;
                    if (nt == col + 1) { int nt1 = icn_nxttab(nt, stops, nstops, gap); if (nt1 > target) do_tab = 0; }
                    if (do_tab) {
                        while (nt <= target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]='\t'; col = nt; nt = icn_nxttab(col, stops, nstops, gap); }
                        while (col++ < target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; }
                    } else {
                        while (col < target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; col++; }
                    }
                } else {
                    if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' ';
                }
                col = target; i = j;
            } else if (c == '\t') {
                col = icn_nxttab(col, stops, nstops, gap);
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]='\t'; i++;
            } else if (c == '\b') {
                if (col > 1) col--;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c; i++;
            } else if (c == '\n' || c == '\r') {
                col = 1;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c; i++;
            } else {
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c;
                if (isprint((unsigned char)c)) col++;
                i++;
            }
        }
        buf[bi]='\0'; *out=BSTRVAL(buf, bi); return 1;
    }
    L_bidjmp_5747: ;
    if ((_bid == BID_abs) && nargs == 1) {
        extern void rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes);
        DESCR_t av = args[0]; DESCR_t nv;
        rt_coerce_num2_d(&av, &av, &nv, 0);
        if (IS_REAL_fn(nv)) { *out = REALVAL(fabs(nv.r)); return 1; }
        *out = INTVAL(nv.i < 0 ? -nv.i : nv.i); return 1;
    }
    L_bidjmp_5754: ;
    if ((_bid == BID_max) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int gt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                ? ((IS_REAL_fn(best)?best.r:(double)best.i) < (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                : (best.i < cv.i);
            if (gt) best = cv;
        }
        *out = best; return 1;
    }
    L_bidjmp_5765: ;
    if ((_bid == BID_min) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int lt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                ? ((IS_REAL_fn(best)?best.r:(double)best.i) > (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                : (best.i > cv.i);
            if (lt) best = cv;
        }
        *out = best; return 1;
    }
#define TONUM(av) (IS_REAL_fn(av) ? (av).r : IS_INT_fn(av) ? (double)(av).i : ((av).v==DT_S && (av).s ? strtod((av).s,NULL) : 0.0))
    L_bidjmp_5777: ;
    if ((_bid == BID_sqrt) && nargs >= 1) {
        DESCR_t av = args[0];
        double v = TONUM(av);
        *out = REALVAL(sqrt(v)); return 1;
    }
#define MATH1(fname, cfn) \
    if (!strcmp(fn, fname) && nargs >= 1) { double _v = TONUM(args[0]); *out = REALVAL(cfn(_v)); return 1; }
    MATH1("sin",  sin)
    MATH1("cos",  cos)
    MATH1("tan",  tan)
    MATH1("asin", asin)
    MATH1("acos", acos)
    MATH1("exp",  exp)
#undef MATH1
    L_bidjmp_5791: ;
    if ((_bid == BID_atan) && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double v2 = TONUM(args[1]); *out = REALVAL(atan2(v,v2)); }
        else *out = REALVAL(atan(v));
        return 1;
    }
    L_bidjmp_5797: ;
    if ((_bid == BID_log) && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double base = TONUM(args[1]); *out = REALVAL(log(v)/log(base)); }
        else *out = REALVAL(log(v));
        return 1;
    }
    L_bidjmp_5803: ;
    if ((_bid == BID_dtor) && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*3.14159265358979323846/180.0); return 1; }
    L_bidjmp_5804: ;
    if ((_bid == BID_rtod) && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*180.0/3.14159265358979323846); return 1; }
    L_bidjmp_5805: ;
    if ((_bid == BID_iand)  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a&b); return 1; }
    L_bidjmp_5806: ;
    if ((_bid == BID_ior)   && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a|b); return 1; }
    L_bidjmp_5807: ;
    if ((_bid == BID_ixor)  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a^b); return 1; }
    L_bidjmp_5808: ;
    if ((_bid == BID_ishift)&& nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(b>=0?a<<b:a>>(-b)); return 1; }
    L_bidjmp_5809: ;
    if ((_bid == BID_icom)  && nargs==1) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r; *out=INTVAL(~a); return 1; }
#undef TONUM
    L_bidjmp_5811: ;
    if ((_bid == BID_copy) && nargs == 1) {
        DESCR_t src = args[0];
        if (src.v == DT_T && src.tbl) {
            TBBLK_t *nt = table_new();
            nt->dflt = src.tbl->dflt;
            nt->init = src.tbl->init;
            nt->inc  = src.tbl->inc;
            nt->is_set = src.tbl->is_set;
            { TBPAIR_t *p; TBL_FOREACH(src.tbl, p) table_set_descr_d(nt, p->key_descr, p->val); }
            DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = nt;
            *out = d; return 1;
        }
        if (src.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(src, "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                DESCR_t ea = FIELD_GET_fn(src, "frame_elems");
                int n = (int)FIELD_GET_fn(src, "frame_size").i;
                DESCR_t *src_elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                DESCR_t *new_elems = (DESCR_t *)rt_ws_alloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t));
                if (src_elems && n > 0) memcpy(new_elems, src_elems, (size_t)n * sizeof(DESCR_t));
                DESCR_t eptr; eptr.v = DT_DATA; eptr.slen = 0; eptr.ptr = (void *)new_elems;
                *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n));
                return 1;
            }
        }
        *out = src; return 1;
    }
    L_bidjmp_5840: ;
    if ((_bid == BID_list) && nargs >= 0) {
        int n = 0;
        DESCR_t init = NULVCL;
        if (nargs >= 1) {
            DESCR_t nv = args[0];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) {
                if (IS_INT_fn(nv)) n = (int)nv.i;
                else if (IS_REAL_fn(nv)) n = (int)nv.r;
                else { *out = FAILDESCR; return 1; }
                if (n < 0) { *out = FAILDESCR; return 1; }
            }
        }
        if (nargs >= 2) {
            DESCR_t iv = args[1];
            if (!IS_FAIL_fn(iv)) init = iv;
        }
        static int list_reg2 = 0;
        if (!list_reg2) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_reg2 = 1; }
        DESCR_t *elems = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
        for (int i = 0; i < n; i++) elems[i] = init;
        DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
        *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n));
        return 1;
    }
    L_bidjmp_5864: ;
    if ((_bid == BID_table) && nargs <= 2) {
        TBBLK_t *tbl = table_new();
        if (nargs == 1) {
            tbl->dflt = args[0];
        } else {
            tbl->dflt = NULVCL;
        }
        DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = tbl;
        *out = d; return 1;
    }
    L_bidjmp_5874: ;
    if ((_bid == BID_read) && nargs == 0) {
        char buf[4096];
        if (!fgets(buf, sizeof buf, stdin)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        char *r = rt_ws_alloc(len + 1); memcpy(r, buf, len + 1);
        *out = STRVAL(r); return 1;
    }
    L_bidjmp_5883: ;
    if ((_bid == BID_reads) && nargs == 1 && (IS_INT_fn(args[0]) || IS_REAL_fn(args[0]))) {
        DESCR_t nd = args[0];
        int n = (int)to_int(nd);
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, stdin);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    L_bidjmp_5894: ;
    if ((_bid == BID_runerr) && nargs >= 1) { long long _ec = IS_INT_fn(args[0]) ? (long long)args[0].i : 500; { extern long g_error; if (g_error != 0) return icn_argtype_raise((int)_ec, (nargs >= 2) ? args[1] : FAILDESCR, out); } fprintf(stderr, "Run-time error %lld\n", _ec); if (nargs >= 2) { DESCR_t _im = FAILDESCR; if (try_call_builtin_by_name("image", args + 1, 1, &_im) && !IS_FAIL_fn(_im)) { const char *_is = VARVAL_fn(_im); fprintf(stderr, "offending value: %s\n", _is ? _is : ""); } } exit(1); }
    L_bidjmp_5895: ;
    if ((_bid == BID_stop)) { for (int _si = 0; _si < nargs; _si++) { DESCR_t _a = args[_si]; if (IS_INT_fn(_a)) fprintf(stderr, "%lld", (long long)_a.i); else if (IS_REAL_fn(_a)) { char _rb[64]; icon_real_str(_a.r, _rb, sizeof _rb); fprintf(stderr, "%s", _rb); } else { const char *_s = VARVAL_fn(_a); if (_s) fprintf(stderr, "%s", _s); } } if (nargs) fprintf(stderr, "\n"); exit(1); }
    if ((_bid == BID_exit)) { long long _st = (nargs >= 1 && IS_INT_fn(args[0])) ? (long long)args[0].i : 0; exit((int)_st); }
    extern const char *scan_subj;
    extern int         scan_pos;
    extern int         scan_depth;
    L_bidjmp_5900: ;
    if ((_bid == BID_ICN_SCAN_PUSH) && nargs == 1) {
        const char *s;
        if (IS_REAL_fn(args[0])) { char _rb[64]; icon_real_str(args[0].r,_rb,sizeof _rb); s = rt_ws_strdup(_rb); }
        else { s = VARVAL_fn(args[0]); if (!s) s = ""; }
        scan_depth++;
        scan_subj = rt_ws_strdup(s); scan_pos = 1;
        *out = args[0]; return 1;
    }
    L_bidjmp_5912: ;
    if ((_bid == BID_ICN_SCAN_POP) && nargs == 1) {
        if (scan_depth > 0) scan_depth--;
        *out = args[0]; return 1;
    }
    L_bidjmp_5920: ;
    if ((_bid == BID_any) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        if (i1 >= i2) { *out = FAILDESCR; return 1; }
        if (!strchr(cv, s[i1 - 1])) { *out = FAILDESCR; return 1; }
        *out = INTVAL(i1 + 1); return 1;
    }
    L_bidjmp_5938: ;
    if ((_bid == BID_many) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int p = i1;
        while (p < i2 && strchr(cv, s[p - 1])) p++;
        if (p == i1) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p); return 1;
    }
    L_bidjmp_5958: ;
    if ((_bid == BID_upto) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int p = i1;
        while (p < i2 && !strchr(cv, s[p - 1])) p++;
        if (p >= i2) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p); return 1;
    }
    L_bidjmp_5978: ;
    if ((_bid == BID_tab) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = rt_ws_alloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5991: ;
    if ((_bid == BID_move) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int n = (int)to_int(args[0]);
        int target = scan_pos + n;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = rt_ws_alloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_6004: ;
    if ((_bid == BID_pos) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1 || target != scan_pos) { *out = FAILDESCR; return 1; }
        *out = INTVAL(target); return 1;
    }
    L_bidjmp_6012: ;
    if ((_bid == BID_match) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(s), plen2 = (int)strlen(pat), p0 = i1 - 1;
        if (p0 + plen2 > slen || i1 + plen2 > i2) { *out = FAILDESCR; return 1; }
        if (strncmp(s + p0, pat, (size_t)plen2) != 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(i1 + plen2); return 1;
    }
    L_bidjmp_6033: ;
    if ((_bid == BID_bal) && (scan_pos > 0 || nargs >= 4)) {
        const char *c1 = 0; int c1len = 0; int c1any = 0;
        if (nargs < 1 || IS_FAIL_fn(args[0]) || args[0].v == DT_SNUL) c1any = 1;
        else if (!cset_resolve(args[0], &c1, &c1len)) { *out = FAILDESCR; return 1; }
        const char *c2 = "("; int c2len = 1;
        const char *c3 = ")"; int c3len = 1;
        if (nargs >= 2) { const char *v; int vlen; if (cset_resolve(args[1], &v, &vlen) && vlen > 0) { c2 = v; c2len = vlen; } }
        if (nargs >= 3) { const char *v; int vlen; if (cset_resolve(args[2], &v, &vlen) && vlen > 0) { c3 = v; c3len = vlen; } }
        const char *s; int slen; int p;
        if (nargs >= 4 && !IS_FAIL_fn(args[3]) && args[3].v != DT_SNUL) {
            s = VARVAL_fn(args[3]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 5 && (IS_INT_fn(args[4]) || IS_REAL_fn(args[4]))) ? (int)to_int(args[4]) : 1;
            int i2 = (nargs >= 6 && (IS_INT_fn(args[5]) || IS_REAL_fn(args[5]))) ? (int)to_int(args[5]) : slen + 1;
            if (i1 <= 0) i1 = slen + 1 + i1;
            if (i2 <= 0) i2 = slen + 1 + i2;
            if (i1 < 1 || i1 > slen + 1) { *out = FAILDESCR; return 1; }
            if (i2 - 1 < slen) slen = i2 - 1;
            p = i1 - 1;
        } else {
            s = scan_subj ? scan_subj : ""; slen = (int)strlen(s);
            p = scan_pos - 1;
        }
        long long cnt = 0;
        while (p < slen) {
            unsigned char ch = (unsigned char)s[p];
            if (cnt == 0 && (c1any || cset_has(c1, c1len, ch))) { *out = INTVAL(p + 1); return 1; }
            if (cset_has(c2, c2len, ch)) cnt++;
            else if (cset_has(c3, c3len, ch)) { cnt--; if (cnt < 0) { *out = FAILDESCR; return 1; } }
            p++;
        }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6067: ;
    if ((_bid == BID_find) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *needle = VARVAL_fn(args[0]); if (!needle) { *out = FAILDESCR; return 1; }
        const char *hay    = (nargs >= 2) ? VARVAL_fn(args[1]) : (const char *)0; if (!hay) hay = scan_subj ? scan_subj : "";
        int nlen = (int)strlen(needle), hlen = (int)strlen(hay);
        int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 && nargs < 2 ? scan_pos : 1);
        int i2 = (nargs >= 4) ? (int)args[3].i : hlen + 1;
        if (i1 <= 0 || i1 > hlen + 1) { *out = FAILDESCR; return 1; }
        if (i2 <= 0 || i2 > hlen + 1) i2 = hlen + 1;
        int term = (i2 - 1) - nlen;
        for (int i = i1 - 1; i <= term; i++) {
            if (strncmp(hay + i, needle, nlen) == 0) { *out = INTVAL(i + 1); return 1; }
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6081: ;
    if ((_bid == BID_NONNULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = FAILDESCR; return 1; }
        if (v.v == DT_S && (!v.s || descr_slen(v) == 0)) { *out = FAILDESCR; return 1; }
        *out = v; return 1;
    }
    L_bidjmp_6088: ;
    if ((_bid == BID_ICN_CASE_EQ) && nargs == 2) {
        DESCR_t topic = args[0], val = args[1];
        if (IS_FAIL_fn(topic) || IS_FAIL_fn(val)) { *out = FAILDESCR; return 1; }
        int eq = 0;
        if ((IS_INT_fn(topic) || IS_REAL_fn(topic)) &&
            (IS_INT_fn(val)   || IS_REAL_fn(val))) {
            double tv = IS_REAL_fn(topic) ? topic.r : (double)topic.i;
            double vv = IS_REAL_fn(val)   ? val.r   : (double)val.i;
            eq = (tv == vv);
        } else {
            const char *ts = VARVAL_fn(topic); if (!ts) ts = "";
            const char *vs = VARVAL_fn(val);   if (!vs) vs = "";
            eq = (strcmp(ts, vs) == 0);
        }
        *out = eq ? val : FAILDESCR; return 1;
    }
    L_bidjmp_6104: ;
    if ((_bid == BID_ICN_SWAP_TOP2) && nargs == 2) {
        *out = args[0];
        return 1;
    }
    L_bidjmp_6108: ;
    if ((_bid == BID_ICN_NULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (v.v == DT_S && (!v.s || descr_slen(v) == 0)) { *out = NULVCL; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6115: ;
    if ((_bid == BID_insert) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        DESCR_t vd = (td.tbl && td.tbl->is_set) ? kd : ((nargs >= 3) ? args[2] : NULVCL);
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        table_set_descr_d(td.tbl, kd, vd);
        *out = td; return 1;
    }
    L_bidjmp_6124: ;
    if ((_bid == BID_delete) && nargs >= 1) {
        DESCR_t td = args[0];
        if (nargs == 1 && IS_STR_fn(td)) {
            const char *path = VARVAL_fn(td);
            if (!path || !path[0] || unlink(path) != 0) { *out = FAILDESCR; return 1; }
            *out = NULVCL; return 1;
        }
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        table_delete_d(td.tbl, kd);
        *out = td; return 1;
    }
    L_bidjmp_6132: ;
    if ((_bid == BID_member) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        if (!table_has_d(td.tbl,kd)) { *out=FAILDESCR; return 1; }
        *out = kd; return 1;
    }
    L_bidjmp_6140: ;
    if ((_bid == BID_key) && nargs == 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T || !td.tbl) { *out=FAILDESCR; return 1; }
        for (unsigned _bi=0;_bi<td.tbl->nbuck;_bi++)
            if (td.tbl->buckets[_bi] && td.tbl->buckets[_bi]->len) {
                *out = td.tbl->buckets[_bi]->ent[0].key_descr; return 1;
            }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6149: ;
    if ((_bid == BID___apply__) && nargs == 2) {
        DESCR_t callee = args[0]; DESCR_t lv = args[1];
        if (lv.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(lv,"gen_type");
            if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0) {
                int n=(int)FIELD_GET_fn(lv,"frame_size").i;
                DESCR_t ea=FIELD_GET_fn(lv,"frame_elems");
                DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
                *out = rt_call_value(callee, arr, (arr?n:0)); return 1;
            }
        }
        { DESCR_t a1 = lv; *out = rt_call_value(callee, &a1, 1); return 1; }
    }
    L_bidjmp_6162: ;
    if ((_bid == BID_push) && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            int _nv2 = (nargs > 1) ? nargs - 1 : 1;
            for (int _pi = 0; _pi < _nv2; _pi++) {
                DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                DESCR_t *_nb=rt_ws_alloc((_n+1)*sizeof(DESCR_t));
                _nb[0]=_vd;
                if(_a&&_n>0) memcpy(_nb+1,_a,_n*sizeof(DESCR_t));
                _a=_nb; _n++;
                rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_nb}); rt_lv_set_size(ld,_n); rt_lv_set_cap(ld,_n); }
            *out = ld; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t *nb=rt_ws_alloc((n+1)*sizeof(DESCR_t));
            nb[0]=vd;
            if(old&&n>0) memcpy(nb+1,old,n*sizeof(DESCR_t));
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
            FIELD_SET_fn(ld,"frame_cap",INTVAL(n+1));
        }
        *out = ld; return 1;
    }
    L_bidjmp_6182: ;
    if ((_bid == BID_put) && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            int _nv2 = (nargs > 1) ? nargs - 1 : 1;
            for (int _pi = 0; _pi < _nv2; _pi++) {
                DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                long _c = rt_lv_cap(ld);
                if(_a && _c>=0 && _n<_c){ _a[_n]=_vd; _n++; rt_lv_set_size(ld,_n); continue; }
                long _ncap=(_n>0)?(long)_n*2:8; if(_ncap<_n+1)_ncap=_n+1;
                DESCR_t *_nb=rt_ws_alloc(_ncap*sizeof(DESCR_t));
                if(_a&&_n>0) memcpy(_nb,_a,_n*sizeof(DESCR_t));
                _nb[_n]=_vd; _a=_nb; _n++;
                rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_nb}); rt_lv_set_size(ld,_n); rt_lv_set_cap(ld,_ncap); }
            *out = ld; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t capd=FIELD_GET_fn(ld,"frame_cap");
            long cap=(capd.v==DT_I)?capd.i:-1;
            if(old&&cap>=0&&n<cap){ old[n]=vd; FIELD_SET_fn(ld,"frame_size",INTVAL(n+1)); continue; }
            long ncap=(n>0)?(long)n*2:8; if(ncap<n+1)ncap=n+1;
            DESCR_t *nb=rt_ws_alloc(ncap*sizeof(DESCR_t));
            if(old&&n>0) memcpy(nb,old,n*sizeof(DESCR_t));
            nb[n]=vd;
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
            FIELD_SET_fn(ld,"frame_cap",INTVAL(ncap));
        }
        *out = ld; return 1;
    }
    L_bidjmp_6206: ;
    if ((_bid == BID_get) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[0]; rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_a+1}); rt_lv_set_size(ld,_n-1);
            { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
            *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        { DESCR_t capd=FIELD_GET_fn(ld,"frame_cap"); if(capd.v==DT_I&&capd.i>0) FIELD_SET_fn(ld,"frame_cap",INTVAL(capd.i-1)); }
        *out = ret; return 1;
    }
    L_bidjmp_6221: ;
    if ((_bid == BID_pop) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[0]; rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_a+1}); rt_lv_set_size(ld,_n-1);
            { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
            *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        { DESCR_t capd=FIELD_GET_fn(ld,"frame_cap"); if(capd.v==DT_I&&capd.i>0) FIELD_SET_fn(ld,"frame_cap",INTVAL(capd.i-1)); }
        *out = ret; return 1;
    }
    L_bidjmp_6236: ;
    if ((_bid == BID_pull) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[_n-1]; rt_lv_set_size(ld,_n-1); *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[n-1];
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        *out = ret; return 1;
    }
    L_bidjmp_6249: ;
    if ((_bid == BID_sort) && nargs >= 1 && args[0].v == DT_T && args[0].tbl) {
        TBBLK_t *tb = args[0].tbl;
        int i_mode = (nargs >= 2) ? (int)to_int(args[1]) : 1;
        if (i_mode < 1 || i_mode > 4) i_mode = 1;
        int n = 0;
        { TBPAIR_t *e; TBL_FOREACH(tb, e) n++; }
        TBPAIR_t **ent = rt_ws_alloc((n>0?n:1)*sizeof(TBPAIR_t*));
        { int _k = 0; TBPAIR_t *e; TBL_FOREACH(tb, e) ent[_k++] = e; }
        int by_val = (i_mode % 2 == 0);
        { TBPAIR_t **_tmp = rt_ws_alloc((n>0?n:1)*sizeof(TBPAIR_t*)); sort_msort_pairs(ent, _tmp, n, by_val); }
        extern DESCR_t rt_make_list(DESCR_t *a, int nn);
        if (tb->is_set) {
            DESCR_t *mem = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
            for (int _k = 0; _k < n; _k++) mem[_k] = ent[_k]->key_descr;
            *out = rt_make_list(mem, n); return 1;
        }
        if (i_mode >= 3) {
            DESCR_t *flat = rt_ws_alloc((2*n>0?2*n:1)*sizeof(DESCR_t));
            for (int _k = 0; _k < n; _k++) { flat[2*_k] = ent[_k]->key_descr; flat[2*_k+1] = ent[_k]->val; }
            *out = rt_make_list(flat, 2*n); return 1;
        }
        DESCR_t outer = rt_make_list(NULL, 0);
        DESCR_t *pairs = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
        for (int _k = 0; _k < n; _k++) { DESCR_t pv[2] = { ent[_k]->key_descr, ent[_k]->val }; pairs[_k] = rt_make_list(pv, 2); }
        FIELD_SET_fn(outer, "frame_elems", (DESCR_t){.v=DT_DATA,.ptr=pairs});
        FIELD_SET_fn(outer, "frame_size", INTVAL(n));
        FIELD_SET_fn(outer, "frame_cap", INTVAL(n));
        *out = outer; return 1;
    }
    if (((_bid == BID_sort)&&(nargs==1||nargs==2))||((_bid == BID_sortf)&&nargs==2)) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
        if (n<=0) { *out=rt_make_list(NULL,0); return 1; }
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr) { *out=rt_make_list(NULL,0); return 1; }
        DESCR_t *sorted=rt_ws_alloc(n*sizeof(DESCR_t));
        memcpy(sorted,arr,n*sizeof(DESCR_t));
        int field_idx=((_bid == BID_sortf)&&nargs==2)?(int)to_int(args[1])-1:-1;
        { DESCR_t *_tmp = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t)); sort_msort_descr(sorted, _tmp, n, field_idx); }
        *out=rt_make_list(sorted,n); return 1;
    }
    L_bidjmp_6322: ;
    if ((_bid == BID_FIELD_GET) && nargs == 2) {
        DESCR_t obj  = args[0];
        DESCR_t fname_d = args[1];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
        *out = dat_field_get(fname, obj); return 1;
    }
    L_bidjmp_6330: ;
    if ((_bid == BID_FIELD_SET) && nargs == 3) {
        DESCR_t val    = args[0];
        DESCR_t obj    = args[1];
        DESCR_t fname_d = args[2];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t *data_field_ptr(const char *field, DESCR_t obj);
        DESCR_t *cell = data_field_ptr(fname, obj);
        if (cell) { *cell = val; *out = val; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6341: ;
    if ((_bid == BID_MAKELIST)) {
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
        *out = rt_make_list(args, nargs); return 1;
    }
    L_bidjmp_6345: ;
    if ((_bid == BID_RECORD_REGISTER) && nargs >= 1) {
        extern void record_register(const char *spec);
        const char *spec = VARVAL_fn(args[0]);
        if (spec && *spec) record_register(spec);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6351: ;
    if ((_bid == BID_RECORD_MAKE) && nargs >= 1) {
        const char *rname = VARVAL_fn(args[0]);
        if (!rname || !*rname) { *out=FAILDESCR; return 1; }
        DatType *_dt = dat_find_type(rname);
        if (!_dt) { *out=FAILDESCR; return 1; }
        DESCR_t fargs[FRAME_SLOT_MAX];
        int nf = nargs - 1;
        for (int _j=0;_j<nf&&_j<FRAME_SLOT_MAX;_j++) fargs[_j]=args[1+_j];
        *out = dat_construct(_dt, fargs, nf); return 1;
    }
    L_bidjmp_6361: ;
    if ((_bid == BID_open) && (nargs == 1 || nargs == 2)) {
        const char *path = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!path) { *out = FAILDESCR; return 1; }
        const char *mode = (nargs == 2 && (args[1].v == DT_S||args[1].v == DT_SNUL) && args[1].s)
                           ? args[1].s : "r";
        const char *cmode = "r";
        if (strstr(mode,"w")) cmode = "w";
        else if (strstr(mode,"a")) cmode = "a";
        FILE *fp = fopen(path, cmode);
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        if (idx >= 0 && idx < FH_MAX) fh_name[idx] = rt_ws_strdup(path);
        *out = FHVAL(idx); return 1;
    }
    L_bidjmp_6376: ;
    if ((_bid == BID_remove) && nargs == 1) {
        const char *_rp = VARVAL_fn(args[0]); if (!_rp) _rp = "";
        if (unlink(_rp) == 0) { *out = NULVCL; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6381: ;
    if ((_bid == BID_close) && nargs == 1) {
        if (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) {
            int idx = (int)args[0].i;
            FILE *fp = fh_get(idx);
            if (fp && idx > 2) { fclose(fp); fh_free(idx); }
        }
        *out = args[0]; return 1;
    }
    if ((_bid == BID_read) && nargs == 1) {
        FILE *fp = (args[0].v == DT_SNUL) ? fh_get(0) : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        char buf[4096];
        if (!fgets(buf, sizeof buf, fp)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
    }
    if ((_bid == BID_reads) && nargs >= 1) {
        FILE *fp = (args[0].v == DT_SNUL) ? fh_get(0) : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        int n = (nargs >= 2 && args[1].v != DT_SNUL && !IS_FAIL_fn(args[1])) ? (int)to_int(args[1]) : 1;
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, fp);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    L_bidjmp_6411: ;
    if ((_bid == BID_IDENTICAL) && nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        int same = (a.v == b.v);
        if (same) {
            if      (a.v == DT_I)               same = (a.i == b.i);
            else if (a.v == DT_R)               same = (a.r == b.r);
            else if (a.v == DT_S || a.v == DT_SNUL) {
                int acs = (a.v == DT_S && a.slen == 0xFFFFFFFFu), bcs = (b.v == DT_S && b.slen == 0xFFFFFFFFu);
                same = (acs == bcs) && (a.s == b.s || (a.s && b.s && strcmp(a.s,b.s)==0));
            }
            else if (a.v == DT_E && a.slen == 0xFFFFFFFEu && b.slen == 0xFFFFFFFEu)
                same = (a.s && b.s && strcmp(a.s,b.s)==0);
            else                                same = (a.ptr == b.ptr);
        }
        *out = same ? b : FAILDESCR; return 1;
    }
    L_bidjmp_6423: ;
    if ((_bid == BID_set) && nargs <= 1) {
        TBBLK_t *tbl = table_new();
        tbl->is_set = 1;
        tbl->id = rt_agg_serial_set();
        if (nargs == 1 && args[0].v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(args[0], "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s,"list")==0) {
                DESCR_t ea = FIELD_GET_fn(args[0], "frame_elems");
                int n = (int)FIELD_GET_fn(args[0], "frame_size").i;
                DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                char kb[64];
                if (elems) for (int _i = 0; _i < n; _i++)
                    table_set_descr_d(tbl, elems[_i], elems[_i]);
            }
        }
        *out = TABLE_VAL(tbl); return 1;
    }
    L_bidjmp_6439: ;
    if ((_bid == BID_ASGN) && nargs == 2) {
        DESCR_t rhs = args[0];
        if (IS_FAIL_fn(rhs)) { *out = FAILDESCR; return 1; }
        DESCR_t lref = args[1];
        if (lref.v == DT_S && lref.s) NV_SET_fn(lref.s, rhs);
        *out = rhs; return 1;
    }
    L_bidjmp_6446: ;
    if ((_bid == BID_name) && nargs == 1) {
        DESCR_t a = args[0];
        if (a.v == DT_N && a.slen == 0 && a.s && *a.s) { *out = STRVAL(rt_ws_strdup_c(a.s)); return 1; }
        if (IS_NAMETRAP_fn(a) && a.p) {
            VCELL_t *vc = (VCELL_t *)a.p;
            if (vc->key && *vc->key) { *out = STRVAL(rt_ws_strdup_c(vc->key)); return 1; }
            if (vc->tbl) {
                DESCR_t kn; int r2 = try_call_builtin_by_name("image", &vc->key_d, 1, &kn);
                const char *key = (r2 && kn.v == DT_S && kn.s) ? kn.s : "";
                char sb[192]; snprintf(sb, sizeof sb, "T[%s]", key);
                *out = STRVAL(rt_ws_strdup_c(sb)); return 1;
            }
            if (vc->cellp && vc->pos > 0) {
                char sb[64]; snprintf(sb, sizeof sb, "L[%ld]", (long)vc->pos);
                *out = STRVAL(rt_ws_strdup_c(sb)); return 1;
            }
            if (vc->sv.v == DT_N && (vc->sv.slen == 0 || vc->sv.slen == 2) && vc->len > 0) {
                DESCR_t bn; int r1 = try_call_builtin_by_name("name", &vc->sv, 1, &bn);
                const char *base = (r1 && bn.v == DT_S && bn.s) ? bn.s : "";
                char sb[128]; snprintf(sb, sizeof sb, "%s[%ld:%ld]", base, (long)vc->pos, (long)(vc->pos + vc->len));
                *out = STRVAL(rt_ws_strdup_c(sb)); return 1;
            }
            *out = FAILDESCR; return 1;
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6462: ;
    if ((_bid == BID_variable) && nargs == 1) {
        const char *vname = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!vname) { *out = FAILDESCR; return 1; }
        DESCR_t v = NV_GET_fn(vname);
        *out = IS_FAIL_fn(v) ? FAILDESCR : v; return 1;
    }
    L_bidjmp_6468: ;
    if ((_bid == BID_SNOx24NAME) && nargs == 1) return bn_sno_name(args, nargs, out);
    L_bidjmp_6469: ;
    if ((_bid == BID_ARRAY) && nargs >= 1) {
        extern DESCR_t sno_array_from_proto(const char *proto, DESCR_t init);
        DESCR_t init = (nargs >= 2) ? args[1] : NULVCL;
        char pb[64]; const char *proto;
        if (IS_INT_fn(args[0])) {
            const long long _n = (long long)args[0].i;
            if (_n < 0) { *out = FAILDESCR; return 1; }
            extern ARBLK_t *array_new(int lo, int hi);
            ARBLK_t *_a = array_new(1, (int)_n);
            if (!_a) { *out = FAILDESCR; return 1; }
            if (!(init.v == DT_SNUL && init.slen == 0)) for (long long _k = 0; _k < _n; _k++) _a->data[_k] = init;
            DESCR_t _r; memset(&_r, 0, sizeof _r); _r.v = DT_A; _r.slen = 0; _r.arr = _a;
            *out = _r; return 1;
        }
        proto = VARVAL_fn(args[0]); if (!proto || !*proto) { *out = FAILDESCR; return 1; }
        (void)pb;
        DESCR_t r = sno_array_from_proto(proto, init);
        if (r.v == DT_A && r.arr) ((ARBLK_t *)r.arr)->proto = rt_ws_strdup(proto);
        *out = r; return 1;
    }
    L_bidjmp_6479: ;
    if ((_bid == BID_TABLE) && nargs <= 3) {
        TBBLK_t *tb = table_new_args(nargs >= 1 ? (int)to_int(args[0]) : 0, nargs >= 2 ? (int)to_int(args[1]) : 0);
        if (nargs >= 3) tb->dflt = args[2];
        DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_T; d.slen = 0; d.tbl = tb;
        *out = d; return 1;
    }
    L_bidjmp_6484: ;
    if ((_bid == BID_ITEM) && nargs >= 2) {
        extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx); extern DESCR_t rt_deref(DESCR_t v);
        DESCR_t cur = args[0];
        for (int k = 1; k < nargs; k++) { cur = rt_subscript_var(cur, args[k]); if (IS_FAIL_fn(cur)) { *out = FAILDESCR; return 1; } }
        *out = rt_deref(cur); return 1;
    }
    L_bidjmp_6490: ;
    if (_bid == BID_PROTOTYPE) {
        *out = agg_prototype(nargs >= 1 ? args[0] : NULVCL); return 1;
    }
    L_bidjmp_6499: ;
    if ((_bid == BID_CONVERT) && nargs == 2) {
        char tb[32]; const char *ts = to_cstring(args[1], tb, sizeof tb); if (!ts) ts = "";
        char tu[32]; { int k = 0; for (; ts[k] && k < 31; k++) tu[k] = (ts[k] >= 'a' && ts[k] <= 'z') ? (char)(ts[k] - 32) : ts[k]; tu[k] = 0; }
        DESCR_t a = args[0];
        if (!strcmp(tu,"INTEGER")) {
            if (IS_INT_fn(a)) { *out = a; return 1; }
            if (IS_REAL_fn(a)) { *out = INTVAL((long long)a.r); return 1; }
            const char *sv = VARVAL_fn(a); if (!sv) { *out = FAILDESCR; return 1; }
            char *e = NULL; long long iv = strtoll(sv, &e, 10); if (e && *e == '\0' && e != sv) { *out = INTVAL(iv); return 1; }
            double dv = strtod(sv, &e); if (e && *e == '\0' && e != sv) { *out = INTVAL((long long)dv); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (!strcmp(tu,"REAL")) {
            if (IS_REAL_fn(a)) { *out = a; return 1; }
            if (IS_INT_fn(a)) { *out = REALVAL((double)a.i); return 1; }
            const char *sv = VARVAL_fn(a); if (!sv) { *out = FAILDESCR; return 1; }
            char *e = NULL; double dv = strtod(sv, &e); if (e && *e == '\0' && e != sv) { *out = REALVAL(dv); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (!strcmp(tu,"STRING")) { const char *sv = VARVAL_fn(a); *out = STRVAL(rt_ws_strdup_c(sv ? sv : "")); return 1; }
        return 0;
    }
    L_bidjmp_6521: ;
    if ((_bid == BID_DATA) && nargs == 1) {
        extern DatType *dat_register(const char *spec);
        const char *sp = VARVAL_fn(args[0]); if (!sp || !*sp) { *out = FAILDESCR; return 1; }
        char nb[128]; int k = 0; for (; sp[k] && sp[k] != '(' && k < 127; k++) nb[k] = sp[k]; nb[k] = 0;
        if (nb[0] && sn4_is_system_fn(nb)) { extern int kwb_error(int code, const char *msg); kwb_error(248, "attempted redefinition of system function"); *out = FAILDESCR; return 1; }
        if (!dat_find_type(nb)) dat_register(sp);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6528: ;
    if ((_bid == BID_SNOx24KWSET) && nargs == 2) {
        extern int rt_keyword_write_snobol4(const char *sval, DESCR_t v);
        char kb[64]; const char *kn = to_cstring(args[0], kb, sizeof kb);
        *out = rt_keyword_write_snobol4(kn ? kn : "", args[1]) ? args[1] : FAILDESCR; return 1;
    }
    L_bidjmp_6534: ;
    if ((_bid == BID_SNOx24STMT) && (nargs == 1 || nargs == 2 || nargs == 3)) {
        extern void rt_stmt_enter(long stno, long line);
        long n = IS_INT(args[0]) ? (long)args[0].i : 0;
        long ln = (nargs >= 2 && IS_INT(args[1])) ? (long)args[1].i : 0;
        if (nargs == 3) { extern void rt_stmt_file_init(const char *file); const char *fp = VARVAL_fn(args[2]); rt_stmt_file_init(fp ? fp : ""); }
        rt_stmt_enter(n, ln);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6540: ;
    if ((_bid == BID_SNOx24MKEXPR) && nargs == 1) {
        const char *nm = VARVAL_fn(args[0]); if (!nm) nm = "";
        DESCR_t xd; xd.v = DT_X; xd.slen = (uint32_t)strlen(nm); xd.s = rt_ws_strdup(nm);
        *out = xd; return 1;
    }
    L_bidjmp_6545: ;
    if ((_bid == BID_SNOx24PBK) && nargs == 2) { extern DESCR_t pat_mk_cset(int, const char *); *out = pat_mk_cset((int)to_int(args[0]), VARVAL_fn(args[1])); return 1; }
    L_bidjmp_6546: ;
    if ((_bid == BID_SNOx24PBN) && nargs == 2) { extern DESCR_t pat_mk_num(int, int64_t); *out = pat_mk_num((int)to_int(args[0]), to_int(args[1])); return 1; }
    L_bidjmp_6547: ;
    if ((_bid == BID_SNOx24PB0) && nargs == 1) { extern DESCR_t pat_mk_nil(int); *out = pat_mk_nil((int)to_int(args[0])); return 1; }
    L_bidjmp_6548: ;
    if ((_bid == BID_SNOx24PBC) && nargs == 3) { extern DESCR_t pat_mk_capt(int, const char *, DESCR_t); *out = pat_mk_capt((int)to_int(args[0]), VARVAL_fn(args[1]), args[2]); return 1; }
    L_bidjmp_6549: ;
    if ((_bid == BID_SNOx24PCUR) && nargs == 1) { extern DESCR_t pat_mk_cursor(const char *); *out = pat_mk_cursor(VARVAL_fn(args[0])); return 1; }
    L_bidjmp_6550: ;
    if ((_bid == BID_SNOx24PBALT) && nargs == 2) { extern DESCR_t pat_alt(DESCR_t, DESCR_t); *out = pat_alt(args[0], args[1]); return 1; }
    L_bidjmp_6551: ;
    if ((_bid == BID_SNOx24PARB) && nargs == 1) { extern DESCR_t pat_arbno(DESCR_t); *out = pat_arbno(args[0]); return 1; }
    L_bidjmp_6552: ;
    if ((_bid == BID_SNOx24PFEN) && nargs == 1) { extern DESCR_t pat_fence_p(DESCR_t); *out = pat_fence_p(args[0]); return 1; }
    L_bidjmp_6553: ;
    if ((_bid == BID_SNOx24PDEF) && nargs == 1) { extern DESCR_t pat_defer(const char *); *out = pat_defer(VARVAL_fn(args[0])); return 1; }
    L_bidjmp_6554: ;
    if ((_bid == BID_SNOx24MKPAT) && nargs >= 1) {
        extern void *rt_proc_get_fn(const char *name);
        const char *nm = VARVAL_fn(args[0]); if (!nm) nm = "";
        void *pf = rt_proc_get_fn(nm);
        if (!pf) { fprintf(stderr, "[SNO] SNO$MKPAT: compiled pattern blob '%s' not registered\n", nm); *out = FAILDESCR; return 1; }
        extern void *dtp_wrap_fn_sz(void *, int64_t, int32_t); extern long rt_fn_frame_bytes_known(void *); extern long rt_fn_zstatic_known(void *);
        DESCR_t pd; pd.v = DT_P; pd.slen = 0; pd.p = dtp_wrap_fn_sz(pf, (int64_t)rt_fn_frame_bytes_known(pf), (int32_t)rt_fn_zstatic_known(pf));
        if (nargs >= 2) { extern void rt_patv_freeze(void *, const char *, long); long _n = 0; if (IS_INT_fn(args[1])) _n = (long)args[1].i; else { const char *cs = VARVAL_fn(args[1]); _n = cs ? atol(cs) : 0; } if (_n > 0) rt_patv_freeze(pd.p, nm, _n); }
        *out = pd; return 1;
    }
    L_bidjmp_6563: ;
    if ((_bid == BID_OPSYN) && nargs >= 2) {
        extern DESCR_t opsyn(DESCR_t, DESCR_t, DESCR_t);
        *out = opsyn(args[0], args[1], nargs > 2 ? args[2] : NULVCL); return 1;
    }
    L_bidjmp_6567: ;
    if ((_bid == BID_CODE) && nargs == 1) {
        extern DESCR_t code(const char *);
        const char *cs = VARVAL_fn(args[0]);
        *out = cs ? code(cs) : FAILDESCR; return 1;
    }
    L_bidjmp_6572: ;
    if ((_bid == BID_EVAL) && nargs == 1) {
        extern DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
        DESCR_t av = args[0];
        if (av.v == DT_X) { *out = rt_call_named_proc(av.s ? av.s : "", (DESCR_t *)0, 0); return 1; }
        if (IS_INT_fn(av) || IS_REAL_fn(av)) { *out = av; return 1; }
        if (av.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (av.v == DT_S) { extern DESCR_t EVAL_fn(DESCR_t); *out = EVAL_fn(av); return 1; }
        if (av.v == DT_P) { core_runtime_error(103, "eval argument is not expression"); *out = FAILDESCR; return 1; }
        if (av.v == DT_E) { extern DESCR_t EXPVAL_fn(DESCR_t); *out = EXPVAL_fn(av); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6581: ;
    if ((_bid == BID_VALUE) && nargs == 1) {
        extern DESCR_t NV_GET_fn(const char *); extern DESCR_t rt_deref(DESCR_t);
        extern int rt_dat_field_of_any(const char *);
        if (rt_dat_field_of_any("VALUE")) {
            extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
            *out = dat_field_get("VALUE", args[0]); return 1;
        }
        if (args[0].v == DT_N) { *out = rt_deref(args[0]); return 1; }
        const char *nm = VARVAL_fn(args[0]); if (!nm || !*nm) { *out = FAILDESCR; return 1; }
        *out = NV_GET_fn(nm); return 1;
    }
    L_bidjmp_6587: ;
    if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6588: ;
    if ((_bid == BID_SNOx24WANTNM)) { extern int rt_g_want_name; rt_g_want_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6589: ;
    if ((_bid == BID_APPLY) && nargs >= 1) {
        const char *pn = (args[0].v == DT_N && args[0].slen == 0) ? args[0].s : VARVAL_fn(args[0]);
        if (!pn || !*pn) { *out = FAILDESCR; return 1; }
        extern int rt_proc_is_registered(const char *); extern DESCR_t rt_call_proc_descr(const char *, int); extern DESCR_t g_call_args[];
        if (rt_proc_is_registered(pn)) { int na = nargs - 1; if (na > 64) na = 64; for (int k = 0; k < na; k++) g_call_args[k] = args[k + 1]; *out = rt_call_proc_descr(pn, na); return 1; }
        { extern int rt_g_want_name; extern int rt_dat_field_of_any(const char *);
          extern DESCR_t rt_field_var(const char *field, DESCR_t obj);
          if (rt_g_want_name && nargs >= 2 && args[1].v == DT_DATA && rt_dat_field_of_any(pn)) {
              rt_g_want_name = 0; *out = rt_field_var(pn, args[1]); return 1;
          } }
        return try_call_builtin_by_name(pn, args + 1, nargs - 1, out);
    }
    if (nargs == 1) {
        DESCR_t a = args[0];
        if (IS_FAIL_fn(a)) { *out = FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=a; return 1; }
            const char *s=VARVAL_fn(a); if(!s||!*s){*out=FAILDESCR;return 1;}
            char *e=NULL; long long iv=strtoll(s,&e,10); if(e&&*e=='\0'){*out=INTVAL(iv);return 1;}
            double dv=strtod(s,&e); if(e&&*e=='\0'){*out=REALVAL(dv);return 1;}
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='-' && fn[1]=='\0') {
            _OPCOERCE(a);
            *out = IS_REAL_fn(a) ? REALVAL(-a.r) : INTVAL(-a.i); return 1;
        }
        if (fn[0]=='*' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=INTVAL(1); return 1; }
            if (a.v==DT_DATA && a.u && a.u->type) { *out=INTVAL(a.u->type->nfields); return 1; }
            const char *s=VARVAL_fn(a); *out=INTVAL(s?(long long)strlen(s):0LL); return 1;
        }
        if (fn[0]=='!' && fn[1]=='\0') {
            extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
            if (core_call_registered_fn(fn, args, nargs, out)) return 1;
            core_runtime_error(29, "undefined operator referenced");
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='/' && fn[1]=='\0') {
            if (IS_INT_fn(a))  { *out=(a.i==0)?a:FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=(a.r==0.0)?a:FAILDESCR; return 1; }
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='\\' && fn[1]=='\0') {
            *out=(a.v==DT_SNUL)?FAILDESCR:a; return 1;
        }
        if (fn[0]=='~' && fn[1]=='\0') {
            const char *s=NULL; int slen=-1;
            if (IS_INT_fn(a)) { char *nb=rt_ws_alloc(32); snprintf(nb,32,"%lld",(long long)a.i); s=nb; }
            else if (IS_REAL_fn(a)) { char *nb=rt_ws_alloc(64); icon_real_str(a.r,nb,64); s=nb; }
            else { s=VARVAL_fn(a); if (IS_CSET_fn(a)) slen=kw_cset_len(s); }
            if(!s) s="";
            if (slen<0) slen=(int)strlen(s);
            unsigned char in_set[256]={0}; for (int i=0;i<slen;i++) in_set[(unsigned char)s[i]]=1;
            char *buf=rt_ws_alloc(256); int n=0;
            for(int c=1;c<256;c++) if(!in_set[c]) buf[n++]=(char)c; buf[n]='\0';
            *out=STRVAL(buf); return 1;
        }
        if (fn[0]=='?' && fn[1]=='\0') {
            if (IS_INT_fn(a)) { *out=(a.i>0)?INTVAL((long long)(rand()%(int)a.i)+1):FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=REALVAL((double)rand()/RAND_MAX*a.r); return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { int n=(int)strlen(s); char *ch=rt_ws_alloc(2); ch[0]=s[rand()%n]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
            *out=FAILDESCR; return 1;
        }
    }
    if (nargs == 2) {
        DESCR_t l=args[0], r=args[1];
        if (IS_FAIL_fn(l)||IS_FAIL_fn(r)) { *out=FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=add(l,r); return 1; }
        if (fn[0]=='-' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=sub(l,r); return 1; }
        if (fn[0]=='*' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=mul(l,r); return 1; }
        if (fn[0]=='/' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=DIVIDE_fn(l,r); return 1; }
        if (fn[0]=='%' && fn[1]=='\0') {
            _OPCOERCE(l); _OPCOERCE(r);
            long li=IS_INT_fn(l)?l.i:(long)l.r, ri=IS_INT_fn(r)?r.i:(long)r.r;
            *out=ri?INTVAL(li%ri):FAILDESCR; return 1;
        }
        if (fn[0]=='^' && fn[1]=='\0') {
            _OPCOERCE(l); _OPCOERCE(r);
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i>=0) {
                long long base=l.i, res=1; for(int k=0;k<(int)r.i;k++) res*=base;
                *out=INTVAL(res); return 1;
            }
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i<0) {
                double rv=pow((double)l.i,(double)r.i);
                *out=INTVAL((long long)rv); return 1;
            }
            double base=IS_REAL_fn(l)?l.r:(double)l.i, exp=IS_REAL_fn(r)?r.r:(double)r.i;
            *out=(DESCR_t){.v=DT_R,.r=pow(base,exp)}; return 1;
        }
        if ((_bid == BID_x3C))  _NUMREL(<);
        if ((_bid == BID_x3Cx3D)) _NUMREL(<=);
        if ((_bid == BID_x3E))  _NUMREL(>);
        if ((_bid == BID_x3Ex3D)) _NUMREL(>=);
        if ((_bid == BID_x3D))  _NUMREL(==);
        if ((_bid == BID_x7Ex3D)) _NUMREL(!=);
        if ((_bid == BID_x3Cx3C))  _STRREL(<);
        if ((_bid == BID_x3Cx3Cx3D)) _STRREL(<=);
        if ((_bid == BID_x3Ex3E))  _STRREL(>);
        if ((_bid == BID_x3Ex3Ex3D)) _STRREL(>=);
        if ((_bid == BID_x3Dx3D))  _STRREL(==);
        if ((_bid == BID_x7Ex3Dx3D)) _STRREL(!=);
        if ((_bid == BID_x3Dx3Dx3D)) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?r:FAILDESCR; return 1;
        }
        if ((_bid == BID_x7Ex3Dx3Dx3D)) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?FAILDESCR:r; return 1;
        }
        if ((_bid == BID_x5Bx5D)) {
            *out=subscript_get(l,r); return 1;
        }
        if ((_bid == BID_x2Bx2B) || (_bid == BID_x2Dx2D) || (_bid == BID_x2Ax2A)) {
            if (l.v == DT_T && l.tbl && l.tbl->is_set && r.v == DT_T && r.tbl && r.tbl->is_set) {
                if (fn[0]=='+') *out=TABLE_VAL(set_union(l.tbl, r.tbl));
                else if (fn[1]=='-') *out=TABLE_VAL(set_diff(l.tbl, r.tbl));
                else *out=TABLE_VAL(set_inter(l.tbl, r.tbl));
                return 1;
            }
            char _lbuf[64], _rbuf[64];
            const char *la, *ra;
            if (IS_INT_fn(l))       { snprintf(_lbuf,sizeof _lbuf,"%lld",(long long)l.i); la=_lbuf; }
            else if (IS_REAL_fn(l)) { icon_real_str(l.r,_lbuf,sizeof _lbuf); la=_lbuf; }
            else                    { la=VARVAL_fn(l); if(!la) la=""; }
            if (IS_INT_fn(r))       { snprintf(_rbuf,sizeof _rbuf,"%lld",(long long)r.i); ra=_rbuf; }
            else if (IS_REAL_fn(r)) { icon_real_str(r.r,_rbuf,sizeof _rbuf); ra=_rbuf; }
            else                    { ra=VARVAL_fn(r); if(!ra) ra=""; }
            int lalen = IS_CSET_fn(l) ? kw_cset_len(la) : -1; if (lalen < 0) lalen = (int)strlen(la);
            int ralen = IS_CSET_fn(r) ? kw_cset_len(ra) : -1; if (ralen < 0) ralen = (int)strlen(ra);
            int outlen; const char *ur;
            if (fn[0]=='+') { ur=cset_union(la,lalen,ra,ralen,&outlen); *out=CSETVAL(cset_canonical(ur, outlen)); }
            else if (fn[1]=='-') { ur=cset_diff(la,lalen,ra,ralen,&outlen); *out=CSETVAL(cset_canonical(ur, outlen)); }
            else { ur=cset_inter(la,lalen,ra,ralen,&outlen); *out=CSETVAL(cset_canonical(ur, outlen)); }
            return 1;
        }
    }
#undef _OPCOERCE
#undef _NUMREL
#undef _STRREL
    {
        DatType *_rdt = dat_find_type(fn);
        if (_rdt) { *out = dat_construct(_rdt, args, nargs); return 1; }
    }
    if (script_try_call_builtin_by_name(fn, args, nargs, out)) return 1;
    { DatType *dt = dat_find_type(fn);
      if (dt && nargs <= dt->nfields) {
          DESCR_t fv[64]; int nf = dt->nfields > 64 ? 64 : dt->nfields;
          for (int i = 0; i < nf; i++) fv[i] = (i < nargs) ? args[i] : NULVCL;
          *out = dat_construct(dt, fv, nf); return 1;
      } }
    { extern int rt_dat_field_of_any(const char *);
      if (nargs == 1 && rt_dat_field_of_any(fn)) {
          extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
          *out = dat_field_get(fn, args[0]); return 1;
      } }
    if (!_dx_skip_syn) { extern const char *rt_builtin_synonym(const char *);
      const char *syn = rt_builtin_synonym(fn);
      if (syn) { if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 2; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = syn; }
        return try_call_builtin_by_name(syn, args, nargs, out); }
      if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 0; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; } }
    { extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
      char c0 = fn[0];
      if (c0 && !((c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z') || c0 == '_')
          && core_call_registered_fn(fn, args, nargs, out)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_dat_field_of_any(const char *name) {
    extern int dat_type_count(void); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
    if (!name || !name[0]) return 0;
    for (int c = 0; c < dat_type_count(); c++) for (int f = 0; f < dat_type_nfields(c); f++) { const char *fn2 = dat_type_field(c, f); if (fn2 && !strcmp(fn2, name)) return 1; }
    return 0;
}
#define RT_SYN_MAX 64
static const char *g_rt_syn_new[RT_SYN_MAX]; static const char *g_rt_syn_old[RT_SYN_MAX]; static int g_rt_syn_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_builtin_synonym_add(const char *newname, const char *oldname) {
    if (!newname || !oldname || g_rt_syn_n >= RT_SYN_MAX) return;
    rt_dtax_gen++;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(g_rt_syn_new[i], newname)) { g_rt_syn_old[i] = oldname; return; }
    g_rt_syn_new[g_rt_syn_n] = newname; g_rt_syn_old[g_rt_syn_n] = oldname; g_rt_syn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_builtin_synonym(const char *name) {
    if (!name) return (const char *)0;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(g_rt_syn_new[i], name)) return g_rt_syn_old[i];
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sno_array_from_proto(const char *proto, DESCR_t init) {
    extern ARBLK_t *array_new(int lo, int hi);
    if (!proto || !*proto) return FAILDESCR;
    char buf[128]; int bn = 0;
    for (const char *q = proto; *q && *q != ',' && bn < 127; q++) buf[bn++] = *q;
    buf[bn] = 0;
    const char *rest = proto[bn] == ',' ? proto + bn + 1 : (const char *)0;
    long lo = 1, hi;
    char *colon = strchr(buf, ':');
    if (colon) { *colon = 0; lo = strtol(buf, (char **)0, 10); hi = strtol(colon + 1, (char **)0, 10); }
    else hi = strtol(buf, (char **)0, 10);
    if (hi < lo - 1) return FAILDESCR;
    ARBLK_t *a = array_new((int)lo, (int)hi);
    if (!a) return FAILDESCR;
    int n = (int)(hi - lo + 1);
    for (int k = 0; k < n; k++) a->data[k] = rest ? sno_array_from_proto(rest, init) : init;
    DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_A; d.slen = 0; d.arr = a;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void * rt_pl_db_get(void *, int64_t);
extern int rt_pl_db_assert(void *, void *, int);
extern int rt_pl_db_erase(void *, int);
extern int rt_pl_db_abolish(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_db_cell_of(DESCR_t *args, void *root) { DESCR_t k = rt_pl_deref_val(args[0]); if (k.v != DT_I) return (void *)0; return rt_pl_db_get(root, k.i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_nb_set(void *, int64_t, void *);
extern int rt_pl_nb_get_cell(void *, int64_t, void *, pl_tr_ctx_t *);
DESCR_t rt_pl_dop_nb_setval_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { DESCR_t k = rt_pl_deref_val(args[0]);
      if (k.v != DT_I) return FAILDESCR;
      return rt_pl_nb_set(root, k.i, (void *)&args[1]) ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_nb_getval_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    extern void rt_gc_point_arr(DESCR_t *, int, const char **);
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready(); rt_pl_tr_gc_sync(cx->tr); rt_gc_point_arr(args, nargs, (const char **)0);
    { DESCR_t k = rt_pl_deref_val(args[0]); int ok;
      if (k.v != DT_I) { rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
      ok = rt_pl_nb_get_cell(root, k.i, (void *)&args[1], cx);
      rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_nb_getval_guard_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_nb_is_set(void *, int64_t);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    { DESCR_t k = rt_pl_deref_val(args[0]);
      if (k.v != DT_I) return (void *)0;
      if (rt_pl_nb_is_set(root, k.i)) return (void *)0;
      return rt_pl_ball_kind2("existence_error", "variable", args[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_db_recompile(void *, const char *, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_db_add(DESCR_t *args, int nargs, void *root, int prepend) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { void *db = pl_db_cell_of(args, root); if (!db) return FAILDESCR;
      return rt_pl_db_assert(db, (void *)&args[1], prepend) ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_db_assertz_c(DESCR_t *args, int nargs, void *root) { return pl_db_add(args, nargs, root, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_asserta_c(DESCR_t *args, int nargs, void *root) { return pl_db_add(args, nargs, root, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_erase_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    { void *db = pl_db_cell_of(args, root); DESCR_t iv = rt_pl_deref_val(args[1]);
      if (!db || iv.v != DT_I) return FAILDESCR;
      return rt_pl_db_erase(db, (int)iv.i) ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_abolish_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 1) return FAILDESCR;
    { void *db = pl_db_cell_of(args, root); if (!db) return pl_ok();
      return rt_pl_db_abolish(db) ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_pair_parts(DESCR_t *p, DESCR_t *h, DESCR_t *b) {
    DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)p);
    if ((int)d->v != DT_PLREF || pl_arity((pl_cell_t *)d) != 2) return 0;
    { DESCR_t *aa = (DESCR_t *)d->p; if (h) *h = aa[0]; if (b) *b = aa[1]; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_db_match_erase(void *, void *);
DESCR_t rt_pl_dop_db_retractall_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { void *db = pl_db_cell_of(args, root); if (!db) return pl_ok();
      rt_pl_db_match_erase(db, (void *)&args[1]);
      return pl_ok(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_db_killed(void *);
void * rt_pl_dop_db_alive_c(DESCR_t *args, int nargs, void *root) {
    extern void *rt_pl_ball_existence(DESCR_t *, int);
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    { void *db = pl_db_cell_of(args, root);
      if (!db || !rt_pl_db_killed(db)) return (void *)0;
      return rt_pl_ball_existence(&args[1], 1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_ax_zguard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    char ob[64]; const char *op = "is";
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    if (pl_cell_text(args[1], ob, sizeof ob, &op) && op && op[0]) { } else { op = "is"; }
    { DESCR_t d = rt_pl_deref_val(args[0]);
      if (d.v == DT_I && d.i == 0) return rt_pl_ball_eval_error("zero_divisor", op, 2);
      if (d.v == DT_R && d.r == 0.0) return rt_pl_ball_eval_error("zero_divisor", op, 2);
      return (void *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_ax_eguard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_evaluable(const char *, int);
    extern const char *prolog_atom_name(int);
    if (nargs != 1) return (void *)0;
    { DESCR_t v = rt_pl_deref_val(args[0]);
      if (v.v == DT_I || v.v == DT_R) return (void *)0;
      if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) return rt_pl_ball_instantiation();
      if ((int)v.v == DT_A) return rt_pl_ball_evaluable(prolog_atom_name((int)v.i), 0);
      if (v.v == DT_S) return rt_pl_ball_evaluable(v.s, 0);
      if ((int)v.v == DT_PLREF) return rt_pl_ball_evaluable(prolog_atom_name((int)(v.slen >> 16)), (int)(v.slen & 0xFFFFu));
      return rt_pl_ball_evaluable("?", 0); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_iso_unbound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_char_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    char b[64]; const char *s;
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    { DESCR_t v = rt_pl_deref_val(args[0]);
      if (pl_iso_unbound(v)) return rt_pl_ball_instantiation();
      if (pl_cell_text(args[0], b, sizeof b, &s) && s && s[0] && !s[1]) return (void *)0;
      return rt_pl_ball_kind2("type_error", "character", v); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_between_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    if (nargs != 3) return (void *)0;
    pl_atoms_ready();
    { DESCR_t lo = rt_pl_deref_val(args[0]); DESCR_t hi = rt_pl_deref_val(args[1]); DESCR_t xv = rt_pl_deref_val(args[2]);
      if (pl_iso_unbound(lo)) return rt_pl_ball_instantiation();
      if (lo.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", lo);
      if (pl_iso_unbound(hi)) return rt_pl_ball_instantiation();
      if (hi.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", hi);
      if (!pl_iso_unbound(xv) && xv.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", xv);
      return (void *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_anum_list_kind(DESCR_t d) {
    DESCR_t cur = rt_pl_deref_val(d);
    for (;;) {
        if (pl_is_nil(cur)) return 1;
        if (pl_iso_unbound(cur)) return 0;
        if (!pl_is_cons(cur)) return -1;
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_anum_is_text(DESCR_t d) { return pl_atom_str(d) != (const char *)0; }
static int pl_anum_is_num(DESCR_t d) { return d.v == DT_I || d.v == DT_R; }
static int pl_anum_is_compound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLREF && (int)(d.slen & 0xFFFFu) > 0; }
static int pl_anum_code_ok(long c) { return c >= 0 && c <= 255; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_elems(DESCR_t lst, int codes) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    DESCR_t cur = rt_pl_deref_val(lst);
    while (pl_is_cons(cur)) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (pl_iso_unbound(e)) return rt_pl_ball_instantiation();
        if (codes) {
            if (e.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", e);
            if (!pl_anum_code_ok((long)e.i)) return rt_pl_ball_kind1("representation_error", "character_code"); }
        else {
            const char *es = pl_atom_str(e);
            if (!es || !es[0] || es[1]) return rt_pl_ball_kind2("type_error", "character", e); }
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_number_syntax(DESCR_t lst, int codes) {
    extern void *rt_pl_ball_kind1(const char *, const char *);
    char buf[512]; size_t k = 0; DESCR_t cur = rt_pl_deref_val(lst);
    while (pl_is_cons(cur)) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (codes) { if (e.v != DT_I) return (void *)0; if (k + 1 >= sizeof buf) return (void *)0; buf[k++] = (char)e.i; }
        else { const char *es = pl_atom_str(e); if (!es || !es[0] || es[1]) return (void *)0; if (k + 1 >= sizeof buf) return (void *)0; buf[k++] = es[0]; }
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
    buf[k] = '\0';
    { const char *p = buf; char *end = (char *)0; while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
      if (!*p) return (void *)0;
      (void)strtod(p, &end);
      if (!end || end == p || !*end) return (void *)0;
      { const char *q = end; while (*q == ' ' || *q == '\t' || *q == '\n' || *q == '\r') q++;
        if (!*q) return rt_pl_ball_kind1("syntax_error", "illegal_number"); } }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_text_list_pair(DESCR_t a, DESCR_t l, int codes, const char *atom_type) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    int lk = pl_anum_list_kind(l);
    if (!pl_iso_unbound(a)) {
        if (codes >= 0 && !strcmp(atom_type, "atom") && !pl_anum_is_text(a)) return rt_pl_ball_kind2("type_error", "atom", a);
        if (!strcmp(atom_type, "number") && !pl_anum_is_num(a)) return rt_pl_ball_kind2("type_error", "number", a); }
    if (lk == -1) return rt_pl_ball_kind2("type_error", "list", l);
    { void *b = pl_anum_elems(l, codes); if (b) return b; }
    if (pl_iso_unbound(a) && lk == 0) return rt_pl_ball_instantiation();
    if (pl_iso_unbound(a) && lk == 1 && !strcmp(atom_type, "number")) { void *b = pl_anum_number_syntax(l, codes); if (b) return b; }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_check(const char *nm, DESCR_t *a, int n) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    if (!nm) return (void *)0;
    if (!strcmp(nm, "atom_length") && n == 2) {
        DESCR_t x = rt_pl_deref_val(a[0]), l = rt_pl_deref_val(a[1]);
        if (pl_iso_unbound(x)) return rt_pl_ball_instantiation();
        if (!pl_anum_is_text(x)) return rt_pl_ball_kind2("type_error", "atom", x);
        if (!pl_iso_unbound(l) && l.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", l);
        return (void *)0; }
    if (!strcmp(nm, "number_string") && n == 2) {
        DESCR_t x = rt_pl_deref_val(a[0]), s = rt_pl_deref_val(a[1]);
        if (!pl_iso_unbound(x) && !pl_anum_is_num(x)) return rt_pl_ball_kind2("type_error", "number", x);
        if (pl_iso_unbound(x) && pl_iso_unbound(s)) return rt_pl_ball_instantiation();
        return (void *)0; }
    if (!strcmp(nm, "atom_concat") && n == 3) {
        DESCR_t x = rt_pl_deref_val(a[0]), y = rt_pl_deref_val(a[1]), z = rt_pl_deref_val(a[2]);
        if (!pl_iso_unbound(z) && !pl_anum_is_text(z)) return rt_pl_ball_kind2("type_error", "atom", z);
        if (!pl_iso_unbound(x) && !pl_anum_is_text(x)) return rt_pl_ball_kind2("type_error", "atom", x);
        if (!pl_iso_unbound(y) && !pl_anum_is_text(y)) return rt_pl_ball_kind2("type_error", "atom", y);
        if (pl_iso_unbound(z) && (pl_iso_unbound(x) || pl_iso_unbound(y))) return rt_pl_ball_instantiation();
        return (void *)0; }
    if ((!strcmp(nm, "atom_chars") || !strcmp(nm, "atom_codes")) && n == 2)
        return pl_anum_text_list_pair(rt_pl_deref_val(a[0]), rt_pl_deref_val(a[1]), !strcmp(nm, "atom_codes"), "atom");
    if ((!strcmp(nm, "number_chars") || !strcmp(nm, "number_codes")) && n == 2)
        return pl_anum_text_list_pair(rt_pl_deref_val(a[0]), rt_pl_deref_val(a[1]), !strcmp(nm, "number_codes"), "number");
    if (!strcmp(nm, "char_code") && n == 2) {
        DESCR_t c = rt_pl_deref_val(a[0]), k = rt_pl_deref_val(a[1]);
        if (!pl_iso_unbound(c)) { const char *cs = pl_atom_str(c); if (!cs || !cs[0] || cs[1]) return rt_pl_ball_kind2("type_error", "character", c); }
        if (!pl_iso_unbound(k)) {
            if (k.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", k);
            if (!pl_anum_code_ok((long)k.i)) { extern void *rt_pl_ball_kind1(const char *, const char *); return rt_pl_ball_kind1("representation_error", "character_code"); } }
        if (pl_iso_unbound(c) && pl_iso_unbound(k)) return rt_pl_ball_instantiation();
        return (void *)0; }
    if (!strcmp(nm, "sub_atom") && n == 5) {
        DESCR_t x = rt_pl_deref_val(a[0]), s = rt_pl_deref_val(a[4]);
        if (pl_iso_unbound(x)) return rt_pl_ball_instantiation();
        if (!pl_anum_is_text(x)) return rt_pl_ball_kind2("type_error", "atom", x);
        for (int i = 1; i <= 3; i++) { DESCR_t v = rt_pl_deref_val(a[i]); if (!pl_iso_unbound(v) && v.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", v); }
        if (!pl_iso_unbound(s) && !pl_anum_is_text(s)) return rt_pl_ball_kind2("type_error", "atom", s);
        return (void *)0; }
    if (!strcmp(nm, "arg") && n == 3) {
        DESCR_t k = rt_pl_deref_val(a[0]), t = rt_pl_deref_val(a[1]);
        if (pl_iso_unbound(k) || pl_iso_unbound(t)) return rt_pl_ball_instantiation();
        if (k.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", k);
        if (!pl_anum_is_compound(t)) return rt_pl_ball_kind2("type_error", "compound", t);
        if ((long long)k.i < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", k);
        return (void *)0; }
    if (!strcmp(nm, "functor") && n == 3) {
        DESCR_t t = rt_pl_deref_val(a[0]), f = rt_pl_deref_val(a[1]), r = rt_pl_deref_val(a[2]);
        if (!pl_iso_unbound(t)) return (void *)0;
        if (pl_iso_unbound(f) || pl_iso_unbound(r)) return rt_pl_ball_instantiation();
        if (r.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", r);
        if (pl_anum_is_compound(f)) return rt_pl_ball_kind2("type_error", "atomic", f);
        if ((long long)r.i < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", r);
        if ((long long)r.i > 0 && !pl_anum_is_text(f)) return rt_pl_ball_kind2("type_error", "atom", f);
        return (void *)0; }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_guard_n(DESCR_t *args, int nargs, int want) {
    char nb[64]; const char *nm;
    if (nargs != want + 1) return (void *)0;
    pl_atoms_ready();
    if (!pl_cell_text(args[0], nb, sizeof nb, &nm) || !nm || !nm[0]) return (void *)0;
    return pl_anum_check(nm, &args[1], want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_anum_guard2_c(DESCR_t *args, int nargs) { return pl_anum_guard_n(args, nargs, 2); }
void * rt_pl_dop_anum_guard3_c(DESCR_t *args, int nargs) { return pl_anum_guard_n(args, nargs, 3); }
void * rt_pl_dop_anum_guard5_c(DESCR_t *args, int nargs) { return pl_anum_guard_n(args, nargs, 5); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_stream_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
    extern FILE *fh_get(int);
    char db[64]; const char *dop = "output";
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    if (pl_cell_text(args[1], db, sizeof db, &dop) && dop && dop[0]) { } else { dop = "output"; }
    { int out = !strcmp(dop, "output");
      DESCR_t d = rt_pl_deref_val(args[0]);
      if (pl_iso_unbound(d)) return rt_pl_ball_instantiation();
      { const char *nm = pl_atom_str(d);
        if (nm) {
            if (!strcmp(nm, "user_input"))  return out ? rt_pl_ball_permission3("output", "stream", d) : (void *)0;
            if (!strcmp(nm, "user_output")) return out ? (void *)0 : rt_pl_ball_permission3("input", "stream", d);
            if (!strcmp(nm, "user_error"))  return out ? (void *)0 : rt_pl_ball_permission3("input", "stream", d);
            return rt_pl_ball_kind2("existence_error", "stream", d);
        } }
      if (d.v == (DTYPE_t)DT_PLREF && (int)(d.slen >> 16) == prolog_atom_intern("$stream") && (d.slen & 0xFFFFu) == 1) {
          DESCR_t a = rt_pl_deref_val(((DESCR_t *)d.p)[0]);
          if (a.v == DT_I && fh_get((int)a.i)) return (void *)0;
          return rt_pl_ball_kind2("existence_error", "stream", d);
      }
      return rt_pl_ball_kind2("domain_error", "stream_or_alias", d); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_curstream_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    { DESCR_t d = rt_pl_deref_val(args[0]);
      if (pl_iso_unbound(d)) return (void *)0;
      if (d.v == (DTYPE_t)DT_PLREF && (int)(d.slen >> 16) == prolog_atom_intern("$stream") && (d.slen & 0xFFFFu) == 1) return (void *)0;
      return rt_pl_ball_kind2("domain_error", "stream", d); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { void *addr[256]; char *nm[256]; int n; } pl_ctv_t;
static tree_t * pl_cell_tree(DESCR_t *c, pl_ctv_t *vt) {
    extern const char *prolog_atom_name(int);
    DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)c);
    if (pl_cell_unbound(d)) {
        for (int i = 0; i < vt->n; i++) if (vt->addr[i] == (void *)d) { tree_t *v = ast_node_new(TT_VAR); v->v.sval = vt->nm[i]; return v; }
        { char b[24]; tree_t *v = ast_node_new(TT_VAR); snprintf(b, sizeof b, "_A%d", vt->n);
          if (vt->n < 256) { vt->addr[vt->n] = (void *)d; vt->nm[vt->n] = rt_ws_strdup_c(b); v->v.sval = vt->nm[vt->n]; vt->n++; } else v->v.sval = rt_ws_strdup_c(b);
          return v; }
    }
    if ((int)d->v == DT_I) { tree_t *t = ast_node_new(TT_ILIT); t->v.ival = d->i; return t; }
    if ((int)d->v == DT_R) { tree_t *t = ast_node_new(TT_FLIT); t->v.dval = d->r; return t; }
    if ((int)d->v == DT_A) { tree_t *t = ast_node_new(TT_QLIT); t->v.sval = rt_ws_strdup_c(prolog_atom_name((int)d->i)); return t; }
    if ((int)d->v == DT_S || (int)d->v == DT_SNUL) { tree_t *t = ast_node_new(TT_QLIT); t->v.sval = rt_ws_strdup_c(d->s ? d->s : ""); return t; }
    if ((int)d->v == DT_PLREF) {
        int fn = plc_functor((pl_cell_t *)d), ar = pl_arity((pl_cell_t *)d);
        const char *nm = prolog_atom_name(fn); DESCR_t *aa = (DESCR_t *)d->p;
        if (nm && !strcmp(nm, ".") && ar == 2) {
            tree_t *lst = ast_node_new(TT_MAKELIST); DESCR_t *cur = d; int guard = 0;
            while (guard++ < 100000) {
                DESCR_t *cd = (DESCR_t *)pl_deref((pl_cell_t *)cur);
                if ((int)cd->v == DT_PLREF && pl_arity((pl_cell_t *)cd) == 2 && plc_functor((pl_cell_t *)cd) == fn) {
                    DESCR_t *kk = (DESCR_t *)cd->p; ast_push(lst, pl_cell_tree(&kk[0], vt)); cur = &kk[1]; continue; }
                { const char *tn = ((int)cd->v == DT_A) ? prolog_atom_name((int)cd->i) : (((int)cd->v == DT_S) ? cd->s : (const char *)0);
                  if (tn && !strcmp(tn, "[]")) { lst->v.ival = 0; return lst; }
                  ast_push(lst, pl_cell_tree(cur, vt)); lst->v.ival = 1; return lst; }
            }
            return lst;
        }
        { tree_t *t = ast_node_new(TT_FNC); t->v.sval = rt_ws_strdup_c(nm ? nm : "?");
          for (int i = 0; i < ar; i++) ast_push(t, pl_cell_tree(&aa[i], vt));
          return t; }
    }
    { tree_t *t = ast_node_new(TT_QLIT); t->v.sval = rt_ws_strdup_c("?"); return t; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rt_pl_clause_tree(void *clause_cell) {
    pl_ctv_t vt; vt.n = 0;
    return pl_cell_tree((DESCR_t *)clause_cell, &vt);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_choice_new(const char *key) { tree_t *c = ast_node_new(TT_CHOICE); c->v.sval = rt_ws_strdup_c(key ? key : "?"); return (void *)c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_choice_add(void *choice, void *clause_tree) { if (choice && clause_tree) ast_push((tree_t *)choice, (tree_t *)clause_tree); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_choice_n(void *choice) { return choice ? ((tree_t *)choice)->n : 0; }
