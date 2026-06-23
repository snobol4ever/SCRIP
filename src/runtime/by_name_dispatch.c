#include "by_name_dispatch.h"
#include "builtins/gen_value.h"
#include "builtins/gen_runtime.h"
#include "../driver/driver_private.h"
#include "../parser/raku/re.h"
#include "core.h"
#include "core/utf8.h"
#include "pattern_match.h"
#include "rt/rt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <gc/gc.h>
#include <math.h>
/*--------------------------------------------------------------------------------------------------------------------*/
/* Public: generator builtins (multi-shot / scan-context) + list mutators that need
   rt_call_arr byname routing rather than the deterministic-only known-builtin path. */
int rt_builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq")
        || !strcmp(name, "push") || !strcmp(name, "put");
}
static int builtin_is_generator(const char *name) { return rt_builtin_is_generator(name); }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_is_truthy(DESCR_t v) {
    if (IS_FAIL_fn(v)) return 0;
    if (IS_INT_fn(v))  return v.i != 0;
    if (IS_REAL_fn(v)) return v.r != 0.0;
    if (v.v == DT_SNUL) return 0;
    const char *s = v.s ? v.s : "";
    return s[0] != '\0' && !(s[0] == '0' && s[1] == '\0');
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name)
{
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name))  return 0;
    static const char *known[] = {
        "write", "writes", "stop",
        "integer", "real", "string", "numeric", "char", "ord", "cset",
        "type", "image", "proc", "args", "copy",
        "abs", "sqrt", "sin", "cos", "tan", "exp", "log",
        "max", "min",
        "trim", "reverse", "repl", "map", "left", "center", "right",
        "detab", "entab", "read", "reads",
        "iand", "ior", "ixor", "ishift", "icom",
        "table", "list", "set", "sort", "sortf", "get", "pop", "pull",
        "member", "insert", "delete", "key",
        "[]",
        "MAKELIST",
        "__rk_arr", "arr_get", "arr_set_pure", "arr_init", "arr_last", "array_sort",
        "elems", "push_pure",
        "hash_get", "hash_set_pure", "hash_delete_pure", "hash_exists",
        "__rk_jct_any", "__rk_jct_all", "__rk_jct_one", "__rk_jct_none",
        "obj_new", "meth_call", "field_set",
        "TIME", "DATE",
        NULL
    };
    for (int i = 0; known[i]; i++) if (!strcmp(known[i], name)) return 1;
    {
        if (dat_find_type(name)) return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define SOH '\x01'
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
/*--------------------------------------------------------------------------------------------------------------------*/
static char *rtos(double r, char *buf, size_t cap) {
    gcvt(r, 14, buf); (void)cap; return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *to_cstring(DESCR_t v, char *scratch, size_t scap) {
    if (IS_INT_fn(v))  { return itos((long long)v.i, scratch, scap); }
    if (IS_REAL_fn(v)) { return rtos(v.r, scratch, scap); }
    const char *s = VARVAL_fn(v); return s ? s : "";
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t elem_to_descr(const char *s, size_t slen) {
    char *buf = GC_malloc(slen + 1);
    memcpy(buf, s, slen); buf[slen] = '\0';
    char *ep; long iv = strtol(buf, &ep, 10);
    if (*ep == '\0' && ep > buf) return INTVAL(iv);
    return STRVAL(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int junction_is(DESCR_t v) {
    return (IS_STR_fn(v) || v.v == DT_SNUL) && v.s && v.s[0] == '\x03';
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_num(double a, double b, int op) {
    switch (op) { case TT_EQ: return a == b; case TT_NE: return a != b; case TT_LT: return a < b;
                  case TT_LE: return a <= b; case TT_GT: return a > b; case TT_GE: return a >= b;
                  default: return a == b; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_str(const char *a, const char *b, int op) {
    int c = strcmp(a, b);
    switch (op) { case TT_EQ: return c == 0; case TT_NE: return c != 0; case TT_LT: return c < 0;
                  case TT_LE: return c <= 0; case TT_GT: return c > 0; case TT_GE: return c >= 0;
                  default: return c == 0; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
            char *mb = GC_malloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
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
/*--------------------------------------------------------------------------------------------------------------------*/
#define GRAMMAR_MAX 128
static struct { const char *qname; const char *body; int flavor; } gram_reg[GRAMMAR_MAX];
static int gram_n = 0;
static void gram_set(const char *qname, const char *body, int flavor) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) { gram_reg[i].body = GC_strdup(body); gram_reg[i].flavor = flavor; return; }
    if (gram_n < GRAMMAR_MAX) { gram_reg[gram_n].qname = GC_strdup(qname); gram_reg[gram_n].body = GC_strdup(body); gram_reg[gram_n].flavor = flavor; gram_n++; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gram_get_flavor(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].flavor;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *gram_get(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].body;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void gram_expand(const char *gname, const char *body, int flavor, char *out, int outsz, int depth) {
    int op = 0, n = (int)strlen(body);
    for (int i = 0; i < n && op < outsz - 1; ) {
        char c = body[i];
        if (c == '\\' && i + 1 < n) { out[op++] = c; if (op < outsz - 1) out[op++] = body[i + 1]; i += 2; continue; }
        if (c == '\'' || c == '"') {
            char q = c; out[op++] = c; i++;
            while (i < n && body[i] != q && op < outsz - 1) { if (body[i] == '\\' && i + 1 < n && op < outsz - 2) { out[op++] = body[i]; out[op++] = body[i+1]; i += 2; } else out[op++] = body[i++]; }
            if (i < n && op < outsz - 1) out[op++] = body[i++];
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
                }
            }
        }
        out[op++] = c; i++;
    }
    out[op] = '\0';
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_grammar_register(const char *qname, const char *body, int flavor) { if (qname && body) gram_set(qname, body, flavor); }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_grammar_count(void) { return gram_n; }
/*--------------------------------------------------------------------------------------------------------------------*/
const char *rt_grammar_qname(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].qname : NULL; }
/*--------------------------------------------------------------------------------------------------------------------*/
const char *rt_grammar_body(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].body : NULL; }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_grammar_flavor(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].flavor : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_grammar_has_top(const char *gname) { if (!gname) return 0; char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname); return gram_get(qn) != NULL; }
/*--------------------------------------------------------------------------------------------------------------------*/
static int grammar_parse_core(const char *gname, const char *subj, DESCR_t *out) {
    if (!gname) gname = ""; if (!subj) subj = "";
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
    *out = ok ? STRVAL(GC_strdup(subj)) : NULVCL; return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    if (!meth || !*meth) return 0;
    char sb[64]; const char *s = to_cstring(recv, sb, sizeof sb); if (!s) s = ""; size_t n = strlen(s);
    if (!strcmp(meth, "chars")) { *out = INTVAL((long)utf8_strlen(s)); return 1; }
    if (!strcmp(meth, "uc")) { char *r = (char *)GC_malloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)toupper((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "lc") || !strcmp(meth, "fc")) { char *r = (char *)GC_malloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "tc")) { char *r = (char *)GC_malloc(n + 1); memcpy(r, s, n + 1); if (n > 0) r[0] = (char)toupper((unsigned char)r[0]); *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "tclc")) { char *r = (char *)GC_malloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; if (n > 0) r[0] = (char)toupper((unsigned char)r[0]); *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "flip")) { char *r = (char *)GC_malloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = s[n - 1 - i]; r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "trim")) { size_t a = 0, b = n; while (a < b && isspace((unsigned char)s[a])) a++; while (b > a && isspace((unsigned char)s[b - 1])) b--; char *r = (char *)GC_malloc(b - a + 1); memcpy(r, s + a, b - a); r[b - a] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "Str")) { *out = STRVAL(GC_strdup(s)); return 1; }
    if (!strcmp(meth, "Int")) { if (IS_INT_fn(recv)) { *out = recv; return 1; } if (IS_REAL_fn(recv)) { *out = INTVAL((long)recv.r); return 1; } *out = INTVAL((long)atoll(s)); return 1; }
    if (!strcmp(meth, "contains") && nmargs >= 1) { char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; *out = INTVAL(strstr(s, nd) ? 1 : 0); return 1; }
    if (!strcmp(meth, "index") && nmargs >= 1) { char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; const char *hit = strstr(s, nd); *out = hit ? INTVAL((long)(hit - s)) : NULVCL; return 1; }
    if (!strcmp(meth, "substr") && nmargs >= 1) { long from = IS_INT_fn(margs[0]) ? (long)margs[0].i : atol(to_cstring(margs[0], sb, sizeof sb)); long ln = (nmargs >= 2) ? (IS_INT_fn(margs[1]) ? (long)margs[1].i : atol(to_cstring(margs[1], sb, sizeof sb))) : (long)utf8_strlen(s) - from; if (from < 0) from = 0; if (ln < 0) ln = 0; *out = SUBSTR_fn(recv, INTVAL(from + 1), INTVAL(ln)); return 1; }
    if (!strcmp(meth, "abs")) { if (IS_INT_fn(recv)) { long v = (long)recv.i; *out = INTVAL(v < 0 ? -v : v); } else { *out = REALVAL(fabs(to_real(recv))); } return 1; }
    if (!strcmp(meth, "floor")) { *out = INTVAL((long)floor(to_real(recv))); return 1; }
    if (!strcmp(meth, "ceiling")) { *out = INTVAL((long)ceil(to_real(recv))); return 1; }
    if (!strcmp(meth, "round")) { *out = INTVAL((long)floor(to_real(recv) + 0.5)); return 1; }
    if (!strcmp(meth, "Bool") || !strcmp(meth, "so") || !strcmp(meth, "not")) { int truthy; if (IS_INT_fn(recv)) truthy = (recv.i != 0); else if (IS_REAL_fn(recv)) truthy = (recv.r != 0.0); else truthy = (n > 0); *out = INTVAL(!strcmp(meth, "not") ? (truthy ? 0 : 1) : (truthy ? 1 : 0)); return 1; }
    if ((!strcmp(meth, "succ") || !strcmp(meth, "pred")) && (IS_INT_fn(recv) || IS_REAL_fn(recv))) { int d = !strcmp(meth, "succ") ? 1 : -1; if (IS_INT_fn(recv)) *out = INTVAL((long)recv.i + d); else *out = REALVAL(recv.r + d); return 1; }
    if (!strcmp(meth, "words")) { char *r = (char *)GC_malloc(n + 1); int op = 0, first = 1; size_t i = 0; while (i < n) { while (i < n && isspace((unsigned char)s[i])) i++; if (i >= n) break; if (!first) r[op++] = SOH; first = 0; while (i < n && !isspace((unsigned char)s[i])) r[op++] = s[i++]; } r[op] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "comb")) { char *r = (char *)GC_malloc(2 * n + 1); int op = 0; for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; } r[op] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "split") && nmargs >= 1) { char sept[64]; const char *sep = to_cstring(margs[0], sept, sizeof sept); if (!sep) sep = ""; size_t sl = strlen(sep); char *r = (char *)GC_malloc(2 * n + 2); int op = 0; if (sl == 0) { for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; } } else { const char *p = s, *hit; while ((hit = strstr(p, sep)) != NULL) { size_t sg = (size_t)(hit - p); memcpy(r + op, p, sg); op += (int)sg; r[op++] = SOH; p = hit + sl; } size_t tl = strlen(p); memcpy(r + op, p, tl); op += (int)tl; } r[op] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "chomp")) { size_t b = n; if (b > 0 && (s[b - 1] == '\n' || s[b - 1] == '\r')) b--; char *r = (char *)GC_malloc(b + 1); memcpy(r, s, b); r[b] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "wordcase")) { char *r = (char *)GC_malloc(n + 1); memcpy(r, s, n + 1); int start = 1; for (size_t i = 0; i < n; i++) { if (isspace((unsigned char)r[i])) start = 1; else { if (start) r[i] = (char)toupper((unsigned char)r[i]); start = 0; } } *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "lines")) { char *r = (char *)GC_malloc(2 * n + 1); int op = 0, first = 1; size_t i = 0; while (i < n) { if (!first) r[op++] = SOH; first = 0; while (i < n && s[i] != '\n') { if (s[i] != '\r') r[op++] = s[i]; i++; } if (i < n) i++; } r[op] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "elems")) { if (n == 0) { *out = INTVAL(0); return 1; } int c = 1; for (size_t i = 0; i < n; i++) if (s[i] == SOH) c++; *out = INTVAL(c); return 1; }
    if (!strcmp(meth, "join")) { const char *sep = ""; char jb[64]; if (nmargs >= 1) { sep = to_cstring(margs[0], jb, sizeof jb); if (!sep) sep = ""; } size_t sl = strlen(sep); int nsep = 0; for (size_t i = 0; i < n; i++) if (s[i] == SOH) nsep++; char *r = (char *)GC_malloc(n + (size_t)nsep * sl + 1); int op = 0; for (size_t i = 0; i < n; i++) { if (s[i] == SOH) { memcpy(r + op, sep, sl); op += (int)sl; } else r[op++] = s[i]; } r[op] = '\0'; *out = STRVAL(r); return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static long g_pas_heap_ctr = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *resolve_method_chain(const char *cls, const char *mname, char *buf, int bufsz) {
    extern const char *dat_parent(const char *name);
    extern int rt_proc_has_native_fn(const char *name);
    const char *c = cls;
    while (c && *c) {
        snprintf(buf, bufsz, "%s__%s", c, mname);
        int found = rt_proc_has_native_fn(buf);
        if (!found) for (int pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { found = 1; break; }
        if (found) return buf;
        c = dat_parent(c);
    }
    snprintf(buf, bufsz, "%s__%s", cls, mname);
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int meth_is_user_proc(const char *procname) {
    extern int rt_proc_has_native_fn(const char *name);
    if (procname && rt_proc_has_native_fn(procname)) return 1;
    if (procname) for (int pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, procname)) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static char *pas_nrec_subrec_set(const char *cur, long fi, long ei, const char *val) {
    if (!cur) cur = ""; if (!val) val = ""; if (fi < 0) return GC_strdup(cur); if (ei < 0) ei = 0;
    const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
    for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
    if (!fstart) return GC_strdup(cur);
    size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
    char *field = GC_malloc(flen + 1); memcpy(field, fstart, flen); field[flen] = '\0';
    long nsub = 1; for (size_t j = 0; j < flen; j++) if (field[j] == '\x05') nsub++;
    long want = (ei + 1 > nsub) ? ei + 1 : nsub;
    const char **elems = (const char **)GC_malloc((size_t)want * sizeof(char *));
    const char *p = field; long ix = 0;
    for (;;) { const char *nx = strchr(p, '\x05'); size_t el = nx ? (size_t)(nx - p) : strlen(p); char *e = GC_malloc(el + 1); memcpy(e, p, el); e[el] = '\0'; if (ix < want) elems[ix] = e; ix++; if (!nx) break; p = nx + 1; }
    for (long j = nsub; j < want; j++) elems[j] = "0";
    elems[ei] = val;
    size_t newflen = 0; for (long j = 0; j < want; j++) newflen += strlen(elems[j]); newflen += (size_t)(want - 1);
    char *newf = GC_malloc(newflen + 1); size_t fp = 0; for (long j = 0; j < want; j++) { if (j) newf[fp++] = '\x05'; size_t L = strlen(elems[j]); memcpy(newf + fp, elems[j], L); fp += L; } newf[fp] = '\0';
    size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
    char *o = GC_malloc(pre + fp + post + 1); memcpy(o, cur, pre); memcpy(o + pre, newf, fp); if (fend) memcpy(o + pre + fp, fend, post); o[pre + fp + post] = '\0';
    return o;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn) return 0;
    if (!strcmp(fn, "__pas_sqr") && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d * d; *out = r; return 1; }
        long v = IS_INT_fn(args[0]) ? args[0].i : 0;
        *out = INTVAL(v * v); return 1;
    }
    if (!strcmp(fn, "__pas_alloc") && nargs == 0) {
        char key[32]; g_pas_heap_ctr++; snprintf(key, 32, "__heap_%ld", g_pas_heap_ctr);
        NV_SET_fn(key, INTVAL(0)); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_alloc_rec") && nargs == 1) {
        long nf = IS_INT_fn(args[0]) ? args[0].i : 1; if (nf < 1) nf = 1;
        size_t len = (size_t)(nf * 2 - 1); char *seg = GC_malloc(len + 1); size_t p = 0;
        for (long k = 0; k < nf; k++) { if (k) seg[p++] = SOH; seg[p++] = '0'; } seg[p] = '\0';
        char key[32]; g_pas_heap_ctr++; snprintf(key, 32, "__heap_%ld", g_pas_heap_ctr);
        NV_SET_fn(key, STRVAL(seg)); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_field_set") && nargs == 3) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[2]; return 1; }
        char key[32]; snprintf(key, 32, "__heap_%ld", n);
        DESCR_t recd = NV_GET_fn(key); const char *cur = VARVAL_fn(recd); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        const char *s = cur; long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == idx) { tstart = s; tend = nx; break; } if (!nx) { tstart = NULL; break; } s = nx + 1; k++; }
        if (!tstart) { *out = args[2]; return 1; }
        size_t pre = (size_t)(tstart - cur); size_t post = tend ? strlen(tend) : 0; size_t rvl = strlen(rv);
        char *o = GC_malloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post); o[pre + rvl + post] = '\0';
        NV_SET_fn(key, STRVAL(o)); *out = args[2]; return 1;
    }
    if (!strcmp(fn, "__pas_field_idx_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        char key[32]; snprintf(key, 32, "__heap_%ld", n);
        DESCR_t recd = NV_GET_fn(key); const char *cur = VARVAL_fn(recd); if (!cur) cur = "";
        long fidx = IS_INT_fn(args[1]) ? args[1].i : 0; long eidx = IS_INT_fn(args[2]) ? args[2].i : 0; if (eidx < 1) { *out = args[3]; return 1; }
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fidx) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = args[3]; return 1; }
        size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
        unsigned char ch; if (IS_INT_fn(args[3])) { long cv = args[3].i; if (cv < 0) cv = 0; if (cv > 255) cv = 255; ch = (unsigned char)cv; } else { const char *vs = VARVAL_fn(args[3]); ch = (unsigned char)((vs && vs[0]) ? vs[0] : ' '); }
        size_t nflen = ((size_t)eidx > flen) ? (size_t)eidx : flen; char *nfb = GC_malloc(nflen + 1);
        for (size_t j = 0; j < nflen; j++) nfb[j] = (j < flen) ? fstart[j] : ' '; nfb[eidx - 1] = (char)ch; nfb[nflen] = '\0';
        size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
        char *o = GC_malloc(pre + nflen + post + 1); memcpy(o, cur, pre); memcpy(o + pre, nfb, nflen);
        if (fend) memcpy(o + pre + nflen, fend, post); o[pre + nflen + post] = '\0';
        NV_SET_fn(key, STRVAL(o)); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_deref") && nargs == 1) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        if (n <= 0) { *out = INTVAL(0); return 1; }
        char key[32]; snprintf(key, 32, "__heap_%ld", n);
        *out = NV_GET_fn(key); return 1;
    }
    if (!strcmp(fn, "__pas_deref_set") && nargs == 2) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        if (n > 0) { char key[32]; snprintf(key, 32, "__heap_%ld", n); NV_SET_fn(key, args[1]); }
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
        for (;;) { const char *nx = (const char *)memchr(p, '\x05', (size_t)(pend - p)); const char *eend = nx ? nx : pend; if (ix == ei) { *out = elem_to_descr(p, (size_t)(eend - p)); return 1; } if (!nx) { *out = INTVAL(0); return 1; } p = nx + 1; ix++; }
    }
    if (!strcmp(fn, "__pas_nrec_update") && nargs == 4) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        *out = STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv)); return 1;
    }
    if (!strcmp(fn, "__pas_nrec_deref_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        char key[32]; snprintf(key, 32, "__heap_%ld", n);
        DESCR_t recd = NV_GET_fn(key); const char *cur = VARVAL_fn(recd); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        NV_SET_fn(key, STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv))); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_fassign") && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = STRVAL(GC_strdup(s)); return 1;
    }
    if (!strcmp(fn, "__pas_rewrite") && nargs == 1) {
        extern int fh_alloc(FILE *);
        const char *nm = VARVAL_fn(args[0]); if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "w"); if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "__pas_reset") && nargs == 1) {
        extern int fh_alloc(FILE *);
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
        long e = IS_INT_fn(args[0]) ? args[0].i : -1;
        long s = IS_INT_fn(args[1]) ? args[1].i : 0;
        *out = INTVAL((e >= 0 && e < 64 && ((s >> e) & 1L)) ? 1 : 0); return 1;
    }
    if (!strcmp(fn, "__pas_set")) {
        long s = 0;
        for (int k = 0; k < nargs; k++) { long e = IS_INT_fn(args[k]) ? args[k].i : -1; if (e >= 0 && e < 64) s |= (1L << e); }
        *out = INTVAL(s); return 1;
    }
    if (nargs == 2 && (!strcmp(fn, "__pas_setuni") || !strcmp(fn, "__pas_setint") || !strcmp(fn, "__pas_setdif")
                    || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super"))) {
        long a = IS_INT_fn(args[0]) ? args[0].i : 0;
        long b = IS_INT_fn(args[1]) ? args[1].i : 0;
        long r;
        if      (!strcmp(fn, "__pas_setuni")) r = a | b;
        else if (!strcmp(fn, "__pas_setint")) r = a & b;
        else if (!strcmp(fn, "__pas_setdif")) r = a & ~b;
        else if (!strcmp(fn, "__pas_subset")) r = ((a & ~b) == 0) ? 1 : 0;
        else                                  r = ((b & ~a) == 0) ? 1 : 0;
        *out = INTVAL(r); return 1;
    }
    if (nargs >= 1 && (!strcmp(fn, "__rk_jct_any") || !strcmp(fn, "__rk_jct_all")
                    || !strcmp(fn, "__rk_jct_one") || !strcmp(fn, "__rk_jct_none"))) {
        char flav = !strcmp(fn, "__rk_jct_any") ? 'a'
                  : !strcmp(fn, "__rk_jct_all") ? 'l'
                  : !strcmp(fn, "__rk_jct_one") ? 'o' : 'n';
        size_t total = 2;
        char **ms = GC_malloc((size_t)nargs * sizeof(char *));
        size_t *mlen = GC_malloc((size_t)nargs * sizeof(size_t));
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            size_t L = strlen(cs);
            char *cp = GC_malloc(L + 1); memcpy(cp, cs, L + 1);
            ms[i] = cp; mlen[i] = L; total += 1 + L;
        }
        char *buf = GC_malloc(total + 2);
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
    if (!strcmp(fn, "__rk_arr") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "reverse") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
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
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < listargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (n < 0) n = 0;
        int lo, hi;
        if (is_tail) { lo = (int)(nel - n); if (lo < 0) lo = 0; hi = nel; }
        else         { lo = 0; hi = (int)(n < nel ? n : nel); }
        size_t total = 0; for (int i = lo; i < hi; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = lo; i < hi; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "unique") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
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
                if (!dup && nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
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
    if (!strcmp(fn, "join") && nargs >= 1) {
        char sb[64]; const char *sep = to_cstring(args[0], sb, sizeof sb);
        char *sepd = GC_strdup(sep ? sep : ""); size_t seplen = strlen(sepd);
        char *buf = GC_strdup(""); size_t blen = 0; int first = 1;
        for (int i = 1; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                size_t add = (first ? 0 : seplen) + L;
                char *no = GC_malloc(blen + add + 1);
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
    if (!strcmp(fn, "arr_get") && nargs == 2) {
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
        char *o = GC_malloc((size_t)len + 1);
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
        size_t n = strlen(s); char *o = GC_malloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "lc") || !strcmp(fn, "str_lc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = GC_malloc(n + 1);
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
        char *o = GC_malloc(n + 1); memcpy(o, s, n); o[n] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "array_sort") && nargs == 1) {
        const char *as = VARVAL_fn(args[0]); if (!as || !*as) { *out = STRVAL(GC_strdup("")); return 1; }
        int cnt = 1; for (const char *p = as; *p; p++) if (*p == SOH) cnt++;
        char **elems = GC_malloc((size_t)cnt * sizeof(char*));
        int idx = 0; const char *seg = as;
        do {
            const char *nx = strchr(seg, SOH);
            size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
            char *el = GC_malloc(elen + 1); memcpy(el, seg, elen); el[elen] = '\0';
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
        char *o = GC_malloc(total + 1); o[0] = '\0';
        for (int i = 0; i < cnt; i++) {
            if (i) { size_t ol = strlen(o); o[ol] = SOH; o[ol+1] = '\0'; }
            strcat(o, elems[i]);
        }
        *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "die") || !strcmp(fn, "script_die")) && nargs >= 1) {
        const char *m = VARVAL_fn(args[0]); if (!m) m = "Died";
        extern char g_script_exception[512];
        size_t mlen = strlen(m); if (mlen > 511) mlen = 511;
        memcpy(g_script_exception, m, mlen); g_script_exception[mlen] = '\0';
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_clear") && nargs == 0) {
        extern char g_script_exception[512];
        g_script_exception[0] = '\0';
        *out = STRVAL(GC_strdup("")); return 1;
    }
    if (!strcmp(fn, "exc_check") && nargs == 0) {
        extern char g_script_exception[512];
        if (g_script_exception[0] != '\0') { *out = INTVAL(1); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_get") && nargs == 0) {
        extern char g_script_exception[512];
        *out = STRVAL(GC_strdup(g_script_exception)); return 1;
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
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(GC_strdup("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(GC_strdup("")); return 1; }
        fseek(fp, 0, SEEK_END); long sz = ftell(fp); rewind(fp);
        char *buf = GC_malloc(sz + 1);
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
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(GC_strdup("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_malloc(65536); acc[0] = '\0'; size_t cap = 65536, used = 0; int first = 1;
        char line[4096];
        while (fgets(line, sizeof line, fp)) {
            size_t ll = strlen(line);
            while (ll > 0 && (line[ll-1] == '\n' || line[ll-1] == '\r')) line[--ll] = '\0';
            size_t need = used + ll + 2;
            if (need > cap) { cap = need * 2; char *nb = GC_malloc(cap); memcpy(nb, acc, used); acc = nb; }
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
    if (!strcmp(fn, "field_set") && nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        *out = args[2]; return 1;
    }
    if (!strcmp(fn, "obj_new") && nargs >= 1) {
        const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
        char newproc[256]; resolve_method_chain(cname, "new", newproc, sizeof newproc);
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
        if (mname0 && !strcmp(mname0, "parse") && nargs == 3) {
            const char *gname = VARVAL_fn(args[0]);
            if (gname && rt_grammar_has_top(gname)) { const char *subj = VARVAL_fn(args[2]); return grammar_parse_core(gname, subj, out); }
        }
        if (nargs >= 2 && args[0].v != DT_DATA && rt_str_method(mname0, args[0], &args[2], nargs - 2, out)) return 1;
        if (args[0].v != DT_DATA) {
            const char *tname = VARVAL_fn(args[0]);
            extern int rt_proc_has_native_fn(const char *name);
            if (tname && dat_find_type(tname)) {
                char tproc[256]; resolve_method_chain(tname, mname0, tproc, sizeof tproc);
                if (meth_is_user_proc(tproc)) {
                    int nextra = nargs - 2, total = 1 + nextra;
                    DESCR_t *ca = GC_malloc((size_t)total * sizeof(DESCR_t));
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
        resolve_method_chain(cname, mname, procname, sizeof procname);
        if (nargs == 2 && !meth_is_user_proc(procname)) {
            extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
            DESCR_t *acc = data_field_ptr(mname, args[0]);
            if (acc) { *out = *acc; return 1; }
        }
        int nextra = nargs - 2;
        int total = 1 + nextra;
        DESCR_t *callargs = GC_malloc((size_t)total * sizeof(DESCR_t));
        callargs[0] = args[0];
        for (int k = 0; k < nextra; k++) callargs[1 + k] = args[2 + k];
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
        if (pi >= g_stage2.proc_count) {
            extern DESCR_t g_call_args[];
            extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
            *out = rt_call_proc_descr(procname, total); return 1;
        }
        if (g_stage2.proc_table[pi].bb_idx >= 0) {
            extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
            extern int rt_proc_has_native_fn(const char *name);
            if (rt_proc_has_native_fn(procname)) {
                extern DESCR_t g_call_args[];
                extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
                for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
                *out = rt_call_proc_descr(procname, total); return 1;
            }
            *out = ir_call_proc(pi, callargs, total); return 1;
        }
        *out = proc_table_call(pi, callargs, total); return 1;
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
        if (!nfa) { *out = STRVAL(GC_strdup("")); return 1; }
        int slen = (int)strlen(subj);
        char *acc = GC_malloc((size_t)slen * 4 + 4); acc[0] = '\0';
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
        char *pat  = GC_malloc((size_t)plen + 1); memcpy(pat, tok, (size_t)plen); pat[plen] = '\0';
        char *repl = GC_malloc((size_t)rlen + 1); memcpy(repl, sep1 + 1, (size_t)rlen); repl[rlen] = '\0';
        int global = (*(sep2 + 1) == 'g');
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = args[0]; return 1; }
        int slen = (int)strlen(subj);
        char *res = GC_malloc((size_t)slen * 4 + (size_t)rlen * 8 + 4); res[0] = '\0';
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
        if (!g_match.matched || n < 0 || n >= g_match.ngroups || g_match.group_start[n] < 0) { *out = STRVAL(GC_strdup("")); return 1; }
        int gs = g_match.group_start[n], ge = g_match.group_end[n];
        if (ge < gs) { *out = STRVAL(GC_strdup("")); return 1; }
        int len = ge - gs; char *o = GC_malloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "re_named_capture") && nargs == 1) {
        const char *name = VARVAL_fn(args[0]); if (!name) name = "";
        if (!g_match.matched) { *out = STRVAL(GC_strdup("")); return 1; }
        int g = -1;
        for (int i = 0; i < g_match.ngroups; i++) if (strcmp(g_match.group_name[i], name) == 0) { g = i; break; }
        if (g < 0 || g_match.group_start[g] < 0) { *out = STRVAL(GC_strdup("")); return 1; }
        int gs = g_match.group_start[g], ge = g_match.group_end[g];
        if (ge < gs) { *out = STRVAL(GC_strdup("")); return 1; }
        int len = ge - gs; char *o = GC_malloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "push_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = GC_strdup(cur);
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = GC_malloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "arr_set_pure") && nargs >= 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        size_t rvl = strlen(rv);
        if (!strchr(cur, SOH)) {
            size_t slen = strlen(cur);
            if (idx < 1 || (size_t)idx > slen) { *out = FAILDESCR; return 1; }
            char *buf = GC_malloc(slen * 5 + rvl + 4);
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
        char *o = GC_malloc(total + 1);
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
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(GC_strdup("")); return 1; }
        const char *last = strrchr(cur, SOH);
        if (!last) { *out = STRVAL(GC_strdup("")); return 1; }
        size_t nl = (size_t)(last - cur);
        char *o = GC_malloc(nl + 1); memcpy(o, cur, nl); o[nl] = '\0';
        *out = STRVAL(o); return 1;
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
/*--------------------------------------------------------------------------------------------------------------------*/
#define STX '\x02'
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
        char *o = GC_malloc(total + 1);
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
    char *o = GC_malloc(total + 1);
    memcpy(o, h, hlen);
    if (need_sep) o[hlen] = SOH;
    memcpy(o + hlen + (need_sep ? 1 : 0), key, klen);
    o[hlen + (need_sep ? 1 : 0) + klen] = STX;
    memcpy(o + hlen + (need_sep ? 1 : 0) + klen + 1, val, vlen);
    o[total] = '\0'; return o;
}
/*--------------------------------------------------------------------------------------------------------------------*/
char *script_hash_delete_str(const char *h, const char *key) {
    if (!h || !*h) return GC_strdup("");
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (!pair_start) return GC_strdup(h);
    const char *nx = strchr(pair_start, SOH);
    size_t pre = (size_t)(pair_start - h);
    size_t trim_pre = pre;
    if (pre > 0) trim_pre = pre - 1;
    else if (nx) { nx++; }
    size_t post = nx ? strlen(nx) : 0;
    size_t total = trim_pre + post;
    char *o = GC_malloc(total + 1);
    if (pre > 0) { memcpy(o, h, trim_pre); if (nx) memcpy(o + trim_pre, nx, post); }
    else if (nx) memcpy(o, nx, post);
    o[total] = '\0'; return o;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || nargs < 1) return 0;
    if (args[0].v != DT_S && args[0].v != DT_SNUL) return 0;
    const char *h = VARVAL_fn(args[0]); if (!h) h = "";
    if (!strcmp(fn, "hash_get") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        const char *vstart = hash_find(h, key, NULL);
        if (!vstart) { *out = STRVAL(GC_strdup("")); return 1; }
        const char *vend = strchr(vstart, SOH);
        size_t vlen = vend ? (size_t)(vend - vstart) : strlen(vstart);
        char *v = GC_malloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
        char *ep; long iv = strtol(v, &ep, 10);
        if (*ep == '\0' && ep > v) { *out = INTVAL(iv); return 1; }
        *out = STRVAL(v); return 1;
    }
    if (!strcmp(fn, "hash_exists") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = hash_find(h, key, NULL) ? INTVAL(1) : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "hash_keys") && nargs >= 1) {
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                char *k = GC_malloc(klen + 1); memcpy(k, seg, klen); k[klen] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + klen + 1);
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
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                char *v = GC_malloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + vlen + 1);
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
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
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
                char *pair = GC_malloc(pl + 1);
                memcpy(pair, seg, klen); pair[klen] = ':';
                memcpy(pair + klen + 1, vstart, vlen); pair[pl] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + pl + 1);
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
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#undef STX
#undef SOH
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs) {
    DESCR_t out = FAILDESCR;
    if (!fn) return out;
    if (try_call_builtin_by_name(fn, args, nargs, &out)) return out;
    out = APPLY_fn(fn, args, nargs);
    return out;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int junction_is(DESCR_t v);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
int rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op) {
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
    if (num_rel && (IS_REAL_fn(lhs) || IS_REAL_fn(rhs)) && (IS_INT_fn(lhs) || IS_REAL_fn(lhs)) && (IS_INT_fn(rhs) || IS_REAL_fn(rhs))) {
        double a = to_real(lhs), b = to_real(rhs);
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    if (IS_INT_fn(lhs) && IS_INT_fn(rhs)) {
        int64_t a = lhs.i, b = rhs.i;
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    { const char *a = VARVAL_fn(lhs), *b = VARVAL_fn(rhs); if (!a) a=""; if (!b) b="";
      int c = strcmp(a, b);
      switch (op) { case BINOP_EQ: case BINOP_SEQ: return c==0; case BINOP_NE: case BINOP_SNE: return c!=0;
                    case BINOP_LT: case BINOP_SLT: return c<0;  case BINOP_LE: case BINOP_SLE: return c<=0;
                    case BINOP_GT: case BINOP_SGT: return c>0;  case BINOP_GE: case BINOP_SGE: return c>=0; }
      return 0; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    static const char *builtins[] = {
        "__pas_writeln","__pas_write","__pas_chr","__pas_chrlit","__pas_enum_name","__pas_read_i","__pas_read_c","__pas_readln","__pas_eof","__pas_eoln","__pas_trunc","__pas_abs","__pas_sin","__pas_cos","__pas_exp","__pas_sqrt","__pas_ln","__pas_arctan","__pas_fassign","__pas_rewrite","__pas_reset","__pas_fclose","write","writes","read","reads","close","open","remove","flush",
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
                char *mb = GC_malloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void out_write_descr(FILE *dest, DESCR_t av) {
    if (IS_INT_fn(av))  { fprintf(dest, "%lld", (long long)av.i); return; }
    if (IS_REAL_fn(av)) { char _rb[64]; fprintf(dest, "%s", real_str(av.r,_rb,sizeof _rb)); return; }
    if (IS_CSET_fn(av)) { if (av.s) fwrite(av.s, 1, strlen(av.s), dest); return; }
    const char *s = VARVAL_fn(av); if (s) out_write_str(dest, s);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out)
{
    if (!fn || !out) return 0;
    if (!strcmp(fn, "FAIL"))    { *out = FAILDESCR; return 1; }
    if (!strcmp(fn, "SUCCEED")) { *out = NULVCL;    return 1; }
    if (!strcmp(fn, "__rk_bool") && nargs == 1) {
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
    if (!strcmp(fn, "__rk_bool_val") && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v))  t = 0;
        else if (IS_INT_fn(v))  t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else { const char *s = v.s ? v.s : ""; t = (s[0] != '\0' && !(s[0]=='0' && s[1]=='\0')); }
        *out = INTVAL(t); return 1;
    }
    if (!strcmp(fn, "__pas_chr") && nargs == 1) {
        long long cv = IS_INT_fn(args[0]) ? args[0].i : 0;
        if (cv < 0) cv = 0; if (cv > 255) cv = 255;
        char *s = (char *)GC_malloc(2); s[0] = (char)(unsigned char)cv; s[1] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    if (!strcmp(fn, "__pas_chrlit") && nargs == 1) {
        *out = IS_INT_fn(args[0]) ? args[0] : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "__pas_enum_name") && nargs == 2) {
        long long ord = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        const char *csv = VARVAL_fn(args[1]); if (!csv) csv = "";
        const char *p = csv; long long k = 0;
        while (*p && k < ord) { if (*p == ',') k++; p++; }
        const char *st = p; while (*p && *p != ',') p++;
        size_t L = (size_t)(p - st); char *s = (char *)GC_malloc(L + 1); memcpy(s, st, L); s[L] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    if (!strcmp(fn, "__pas_read_i") && nargs == 0) {
        long long v = 0; scanf(" %lld", &v); *out = INTVAL(v); return 1;
    }
    if (!strcmp(fn, "__pas_read_c") && nargs == 0) {
        int c = getchar(); if (c == EOF) c = 26; *out = INTVAL((long long)(unsigned char)c); return 1;
    }
    if (!strcmp(fn, "__pas_readln") && nargs == 0) {
        int c; while ((c = getchar()) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1;
    }
    if (!strcmp(fn, "__pas_eof") && nargs == 0) {
        int c = getchar();
        if (c == EOF) { *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "__pas_eoln") && nargs == 0) {
        int c = getchar();
        if (c == EOF || c == '\n') { if (c != EOF) ungetc(c, stdin); *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "__pas_trunc") && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = INTVAL((long long)d); return 1;
    }
    if (!strcmp(fn, "__pas_abs") && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d < 0 ? -d : d; *out = r; return 1; }
        long long v = IS_INT_fn(args[0]) ? args[0].i : 0; *out = INTVAL(v < 0 ? -v : v); return 1;
    }
    if (nargs == 1 && (!strcmp(fn,"__pas_sin")||!strcmp(fn,"__pas_cos")||!strcmp(fn,"__pas_exp")||!strcmp(fn,"__pas_sqrt")||!strcmp(fn,"__pas_ln")||!strcmp(fn,"__pas_arctan"))) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        double r;
        if      (!strcmp(fn,"__pas_sin"))    r = sin(d);
        else if (!strcmp(fn,"__pas_cos"))    r = cos(d);
        else if (!strcmp(fn,"__pas_exp"))    r = exp(d);
        else if (!strcmp(fn,"__pas_sqrt"))   r = sqrt(d);
        else if (!strcmp(fn,"__pas_ln"))     r = log(d);
        else                                 r = atan(d);
        DESCR_t rv; rv.v = DT_R; rv.r = r; *out = rv; return 1;
    }
    if (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write")) {
        int nl = (fn[6] == 'w' && fn[7] == 'r' && fn[8] == 'i' && fn[9] == 't' && fn[10] == 'e' && fn[11] == 'l');
        int _start = 0;
        FILE *_dest = stdout;
        if (nargs >= 1 && IS_FH_fn(args[0])) { extern FILE *fh_get(int); FILE *_fp = fh_get((int)args[0].i); if (_fp) _dest = _fp; _start = 2; }
        for (int _pi = _start; _pi + 1 < nargs; _pi += 2) {
            DESCR_t av = args[_pi];
            DESCR_t aw = args[_pi + 1];
            int w = IS_INT_fn(aw) ? (aw.i == -3 ? -3 : (aw.i >= 0 ? (int)aw.i : -1)) : -1;
            if (w == -3) continue;
            if (IS_INT_fn(av)) {
                char _pb[32];
                int _pfmtlen = snprintf(_pb, sizeof _pb, "%lld", (long long)av.i);
                int _fw = (w < 0) ? 10 : (w > _pfmtlen ? w : _pfmtlen);
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
                    else if (w >= 0) { fprintf(_dest, "%*s", w, _ps); }
                    else { fputs(_ps, _dest); }
                }
            }
        }
        if (nl) fputc('\n', _dest);
        *out = NULVCL; return 1;
    }
    if (!strcmp(fn, "write") || !strcmp(fn, "writes")) {
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
            out_write_descr(dest, av);
        }
        if (nl) fputc('\n', dest);
        *out = nargs > start ? args[nargs-1] : (nargs > 0 ? args[0] : NULVCL);
        return 1;
    }
    if (!strcmp(fn,"integer") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av))  { *out = av; return 1; }
        if (IS_REAL_fn(av)) { *out = INTVAL((long long)av.r); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
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
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL((long long)rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"real") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_REAL_fn(av)) { *out = av; return 1; }
        if (IS_INT_fn(av))  { *out = REALVAL((double)av.i); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
        char *end; double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = REALVAL(rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"string") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_STR_fn(av)) { *out = av; return 1; }
        char *buf = GC_malloc(64);
        if (IS_INT_fn(av))       snprintf(buf,64,"%lld",(long long)av.i);
        else if (IS_REAL_fn(av)) { real_str(av.r,buf,64); }
        else { *out = NULVCL; return 1; }
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"numeric") && nargs == 1) {
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
    if (!strcmp(fn,"char") && nargs == 1) {
        DESCR_t av = args[0];
        int n = (int)(IS_INT_fn(av) ? av.i : (long long)strtol(VARVAL_fn(av)?VARVAL_fn(av):"0",NULL,10));
        char *buf = GC_malloc(2); buf[0]=(char)(n&0xFF); buf[1]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"cset") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = av; return 1; }
        char _cbuf[64];
        const char *raw;
        if (IS_INT_fn(av))       { snprintf(_cbuf,sizeof _cbuf,"%lld",(long long)av.i); raw=_cbuf; }
        else if (IS_REAL_fn(av)) { real_str(av.r,_cbuf,sizeof _cbuf); raw=_cbuf; }
        else { raw = VARVAL_fn(av); if (!raw) raw = ""; }
        *out = CSETVAL(cset_canonical(raw)); return 1;
    }
    if (!strcmp(fn,"ord") && nargs == 1) {
        DESCR_t av = args[0];
        const char *s = VARVAL_fn(av); if (!s||!*s) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]); return 1;
    }
    if (!strcmp(fn,"type") && nargs == 1) {
        DESCR_t av = args[0];
        const char *t;
        if (IS_INT_fn(av))       t="integer";
        else if (IS_REAL_fn(av)) t="real";
        else if (av.v==DT_T)     t="table";
        else if (av.v==DT_A)     t="list";
        else if (av.v==DT_DATA)  {
            DESCR_t tag = FIELD_GET_fn(av,"gen_type");
            t = (tag.v==DT_S && tag.s) ? tag.s : "record";
        }
        else if (IS_CSET_fn(av)) t="cset";
        else if (av.v==DT_SNUL)  t="null";
        else t="string";
        *out = STRVAL(t); return 1;
    }
    if (!strcmp(fn,"image") && nargs == 0) {
        *out = STRVAL("&null"); return 1;
    }
    if (!strcmp(fn,"args") && nargs == 1) {
        DESCR_t a = args[0];
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
    if (!strcmp(fn,"proc") && nargs == 2) {
        const char *pname = VARVAL_fn(args[0]);
        int arity = (int)to_int(args[1]);
        if (!pname) { *out = FAILDESCR; return 1; }
        for (int i = 0; i < g_stage2.proc_count; i++) {
            if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, pname) == 0) {
                if (arity < 0 || g_stage2.proc_table[i].nparams == arity || g_stage2.proc_table[i].nparams <= 0) {
                    DESCR_t pv; pv.v = DT_E;
                    pv.slen = (uint32_t)(arity >= 0 ? arity : 0);
                    pv.i    = g_stage2.proc_table[i].entry_pc;
                    *out = pv; return 1;
                }
            }
        }
        *out = STRVAL(GC_strdup(pname)); return 1;
    }
    if (!strcmp(fn,"image") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
        char *buf = GC_malloc(256);
        if (av.v == DT_SNUL)     { *out = STRVAL("&null"); return 1; }
        if (IS_CSET_fn(av)) {
            const char *cs = av.s ? av.s : "";
            const char *kname = kw_cset_name(cs);
            if (kname) { *out = STRVAL(kname); return 1; }
            int clen = (int)strlen(cs);
            char *outs = GC_malloc(clen + 3);
            outs[0] = '\'';
            memcpy(outs+1, cs, clen);
            outs[1+clen] = '\'';
            outs[2+clen] = '\0';
            *out = STRVAL(outs); return 1;
        }
        if (IS_FH_fn(av)) {
            int idx = (int)av.i;
            if (idx >= 0 && idx < FH_MAX && fh_name[idx]) {
                snprintf(buf,256,"file(%s)",fh_name[idx]);
                *out = STRVAL(buf); return 1;
            }
            if (idx == 0) { snprintf(buf,256,"file(&input)");  *out = STRVAL(buf); return 1; }
            if (idx == 1) { snprintf(buf,256,"file(&output)"); *out = STRVAL(buf); return 1; }
            if (idx == 2) { snprintf(buf,256,"file(&errout)"); *out = STRVAL(buf); return 1; }
            snprintf(buf,256,"file(?)"); *out = STRVAL(buf); return 1;
        }
        if (IS_INT_fn(av)) {
            int idx = (int)av.i;
            if (idx >= 0 && idx < FH_MAX && fh_name[idx]) {
                snprintf(buf,256,"file(%s)",fh_name[idx]);
                *out = STRVAL(buf); return 1;
            }
            snprintf(buf,256,"%lld",(long long)av.i); *out = STRVAL(buf); return 1;
        }
        if (IS_REAL_fn(av))      { real_str(av.r,buf,128); *out = STRVAL(buf); return 1; }
        if (av.v==DT_T)          { snprintf(buf,128,"table(%d)",av.tbl?av.tbl->size:0); *out = STRVAL(buf); return 1; }
        if (av.v==DT_DATA && av.u) {
            const char *tname = av.u->type ? av.u->type->name : "record";
            if (strcmp(tname,"list")==0) {
                int cnt = (av.u->type && av.u->type->nfields>=2 && av.u->fields)
                          ? (int)av.u->fields[1].i : 0;
                snprintf(buf,128,"list(%d)",cnt); *out = STRVAL(buf); return 1;
            }
            snprintf(buf,256,"record(%s)",tname); *out = STRVAL(buf); return 1;
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
            int cslen = (int)strlen(cs);
            char *outs = GC_malloc(cslen * 4 + 3);
            int o = 0;
            outs[o++] = '\'';
            for (int i = 0; i < cslen; i++) {
                unsigned char c = (unsigned char)cs[i];
                if (c == '\'') { outs[o++] = '\\'; outs[o++] = '\''; }
                else if (c < 0x20 || c == 0x7f) { o += snprintf(outs+o, 5, "\\x%02x", c); }
                else outs[o++] = (char)c;
            }
            outs[o++] = '\'';
            outs[o] = '\0';
            *out = STRVAL(outs); return 1;
        }
        if (IS_STR_fn(av) && av.s) {
            extern DESCR_t proc_as_value(const char *);
            DESCR_t pv = proc_as_value(av.s);
            if (pv.v == DT_S) {
                snprintf(buf, 128, "function %s", av.s);
                *out = STRVAL(buf); return 1;
            }
        }
        const char *s=VARVAL_fn(av); if (!s) s = "";
        int sl = (int)strlen(s);
        char *outs = GC_malloc(sl*4 + 3);
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
                default:
                    if (c < 0x20 || c == 0x7f) {
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
    if (!strcmp(fn,"image") && nargs == 2) {
        DESCR_t av = args[0];
        if (IS_STR_fn(av) && av.s) {
            char *buf = GC_malloc(64);
            snprintf(buf, 64, "function %s", av.s);
            *out = STRVAL(buf); return 1;
        }
        if (av.v == DT_E) {
            char *buf = GC_malloc(128);
            for (int i=0;i<g_stage2.proc_count;i++)
                if (g_stage2.proc_table[i].entry_pc==(int)av.i)
                    { snprintf(buf,128,"procedure %s",g_stage2.proc_table[i].name); *out=STRVAL(buf); return 1; }
            snprintf(buf,128,"procedure"); *out=STRVAL(buf); return 1;
        }
        DESCR_t one_out = FAILDESCR;
        if (try_call_builtin_by_name("image", args, 1, &one_out))
            { *out = one_out; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"repl") && nargs == 2) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int n=(int)to_int(args[1]); if(n<0)n=0;
        int sl=(int)strlen(s); char *buf=GC_malloc(sl*n+1); buf[0]='\0';
        for(int i=0;i<n;i++) memcpy(buf+i*sl,s,sl); buf[sl*n]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"reverse") && nargs == 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=(int)strlen(s); char *buf=GC_malloc(sl+1);
        for(int i=0;i<sl;i++) buf[i]=s[sl-1-i]; buf[sl]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"map") && nargs >= 1 && nargs <= 3) {
        static const char *UCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const char *LCASE = "abcdefghijklmnopqrstuvwxyz";
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
        int sl=(int)strlen(s); char *buf=GC_malloc(sl+1);
        int fl=(int)strlen(from), tl=(int)strlen(to);
        for (int i=0;i<sl;i++) {
            char c=s[i]; int hit=0;
            for (int j=fl-1;j>=0;j--) {
                if (from[j]==c) { buf[i] = (j<tl) ? to[j] : c; hit=1; break; }
            }
            if (!hit) buf[i]=c;
        }
        buf[sl]='\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"trim") && (nargs == 1 || nargs == 2)) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        const char *cset = " ";
        if (nargs == 2) { DESCR_t cv = args[1]; if (cv.v != DT_SNUL) { const char *cs = VARVAL_fn(cv); if (cs) cset = cs; } }
        int sl=(int)strlen(s);
        while (sl > 0 && strchr(cset, s[sl-1])) sl--;
        char *buf=GC_malloc(sl+1); memcpy(buf,s,sl); buf[sl]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"left") && nargs >= 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=(int)strlen(s);
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
        char *buf=GC_malloc(n+1);
        int copy = sl < n ? sl : n;
        for (int i = 0; i < copy; i++) buf[i] = s[i];
        int rpad = n - copy;
        for (int k = 0; k < rpad; k++) {
            int idx = ((k + fl - rpad) % fl + fl) % fl;
            buf[copy + k] = fill[idx];
        }
        buf[n]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"right") && nargs >= 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=(int)strlen(s);
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
        char *buf=GC_malloc(n+1);
        int pad = n - sl; if (pad < 0) pad = 0;
        for (int i = 0; i < pad; i++) buf[i] = fill[i % fl];
        int srcoff = (sl > n) ? (sl - n) : 0;
        int copy = sl - srcoff; if (pad + copy > n) copy = n - pad;
        for (int i = 0; i < copy; i++) buf[pad + i] = s[srcoff + i];
        buf[n]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"center") && nargs >= 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=(int)strlen(s);
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
        char *buf=GC_malloc(n+1);
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
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"detab") && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) { *out = FAILDESCR; return 1; }
            stops[nstops++] = (int)to_int(args[j]);
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0];
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = GC_malloc(cap); int bi = 0, col = 0;
        for (int i = 0; s[i]; i++) {
            if (s[i] == '\t') {
                int next = -1;
                for (int k = 0; k < nstops; k++) if (stops[k] > col+1) { next=stops[k]; break; }
                if (next < 0) {
                    int base = stops[nstops-1];
                    int beyond = col + 1 - base;
                    next = base + ((beyond / gap) + 1) * gap;
                }
                int sp = next - (col+1);
                while (sp-- > 0) { if (bi>=cap-1){cap*=2;buf=GC_realloc(buf,cap);} buf[bi++]=' '; col++; }
            } else { if (bi>=cap-1){cap*=2;buf=GC_realloc(buf,cap);} buf[bi++]=s[i]; col++; }
        }
        buf[bi]='\0'; *out=STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"entab") && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) { *out = FAILDESCR; return 1; }
            stops[nstops++] = (int)to_int(args[j]);
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0];
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = GC_malloc(cap); int bi = 0, col = 0;
        int slen = (int)strlen(s);
        for (int i = 0; i <= slen; ) {
            if (i < slen && s[i] == ' ') {
                int run_start = col, j = i;
                while (j < slen && s[j]==' ') { j++; col++; }
                int sc = run_start;
                while (sc < col) {
                    int next = -1;
                    for (int k = 0; k < nstops; k++) if (stops[k] > sc+1) { next=stops[k]; break; }
                    if (next < 0) {
                        int base = stops[nstops-1];
                        int beyond = sc + 1 - base;
                        next = base + ((beyond / gap) + 1) * gap;
                    }
                    if (next-1 <= col) { if(bi>=cap-1){cap*=2;buf=GC_realloc(buf,cap);} buf[bi++]='\t'; sc=next-1; }
                    else { if(bi>=cap-1){cap*=2;buf=GC_realloc(buf,cap);} buf[bi++]=' '; sc++; }
                }
                i = j;
            } else {
                if (i < slen) { if(bi>=cap-1){cap*=2;buf=GC_realloc(buf,cap);} buf[bi++]=s[i]; col++; }
                i++;
            }
        }
        buf[bi]='\0'; *out=STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"abs") && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_REAL_fn(av)) { *out = REALVAL(fabs(av.r)); return 1; }
        *out = INTVAL(av.i < 0 ? -av.i : av.i); return 1;
    }
    if (!strcmp(fn,"max") && nargs >= 2) {
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
    if (!strcmp(fn,"min") && nargs >= 2) {
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
    if (!strcmp(fn,"sqrt") && nargs >= 1) {
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
    if (!strcmp(fn,"atan") && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2) { double v2 = TONUM(args[1]); *out = REALVAL(atan2(v,v2)); }
        else *out = REALVAL(atan(v));
        return 1;
    }
    if (!strcmp(fn,"log") && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2) { double base = TONUM(args[1]); *out = REALVAL(log(v)/log(base)); }
        else *out = REALVAL(log(v));
        return 1;
    }
    if (!strcmp(fn,"dtor") && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*3.14159265358979323846/180.0); return 1; }
    if (!strcmp(fn,"rtod") && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*180.0/3.14159265358979323846); return 1; }
    if (!strcmp(fn,"iand")  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a&b); return 1; }
    if (!strcmp(fn,"ior")   && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a|b); return 1; }
    if (!strcmp(fn,"ixor")  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a^b); return 1; }
    if (!strcmp(fn,"ishift")&& nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(b>=0?a<<b:a>>(-b)); return 1; }
    if (!strcmp(fn,"icom")  && nargs==1) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r; *out=INTVAL(~a); return 1; }
#undef TONUM
    if (!strcmp(fn,"copy") && nargs == 1) {
        DESCR_t src = args[0];
        if (src.v == DT_T && src.tbl) {
            TBBLK_t *nt = table_new();
            nt->dflt = src.tbl->dflt;
            nt->init = src.tbl->init;
            nt->inc  = src.tbl->inc;
            for (int b = 0; b < TABLE_BUCKETS; b++)
                for (TBPAIR_t *p = src.tbl->buckets[b]; p; p = p->next)
                    table_set_descr(nt, p->key, p->key_descr, p->val);
            DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = nt;
            *out = d; return 1;
        }
        if (src.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(src, "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                DESCR_t ea = FIELD_GET_fn(src, "frame_elems");
                int n = (int)FIELD_GET_fn(src, "frame_size").i;
                DESCR_t *src_elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                DESCR_t *new_elems = (DESCR_t *)GC_malloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t));
                if (src_elems && n > 0) memcpy(new_elems, src_elems, (size_t)n * sizeof(DESCR_t));
                DESCR_t eptr; eptr.v = DT_DATA; eptr.slen = 0; eptr.ptr = (void *)new_elems;
                *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"));
                return 1;
            }
        }
        *out = src; return 1;
    }
    if (!strcmp(fn,"list") && nargs >= 0) {
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
        if (!list_reg2) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); list_reg2 = 1; }
        DESCR_t *elems = GC_malloc((n>0?n:1)*sizeof(DESCR_t));
        for (int i = 0; i < n; i++) elems[i] = init;
        DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
        *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"));
        return 1;
    }
    if (!strcmp(fn,"table") && nargs <= 2) {
        TBBLK_t *tbl = table_new();
        if (nargs == 1) {
            tbl->dflt = args[0];
        } else {
            tbl->dflt = NULVCL;
        }
        DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = tbl;
        *out = d; return 1;
    }
    if (!strcmp(fn,"read") && nargs == 0) {
        char buf[4096];
        if (!fgets(buf, sizeof buf, stdin)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        char *r = GC_malloc(len + 1); memcpy(r, buf, len + 1);
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(fn,"reads") && nargs == 1) {
        DESCR_t nd = args[0];
        int n = (int)to_int(nd);
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = GC_malloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, stdin);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    if (!strcmp(fn,"stop")) { exit(0); }
    extern const char *scan_subj;
    extern int         scan_pos;
    extern int         scan_depth;
    extern ScanEntry   scan_stack[];
    if (!strcmp(fn,"ICN_SCAN_PUSH") && nargs == 1) {
        const char *s;
        if (IS_REAL_fn(args[0])) { char _rb[64]; real_str(args[0].r,_rb,sizeof _rb); s = GC_strdup(_rb); }
        else { s = VARVAL_fn(args[0]); if (!s) s = ""; }
        if (scan_depth < SCAN_STACK_MAX) {
            scan_stack[scan_depth].subj = scan_subj;
            scan_stack[scan_depth].pos  = scan_pos;
            scan_depth++;
        }
        scan_subj = GC_strdup(s); scan_pos = 1;
        *out = args[0]; return 1;
    }
    if (!strcmp(fn,"ICN_SCAN_POP") && nargs == 1) {
        if (scan_depth > 0) {
            scan_depth--;
            scan_subj = scan_stack[scan_depth].subj;
            scan_pos  = scan_stack[scan_depth].pos;
        }
        *out = args[0]; return 1;
    }
    if (!strcmp(fn,"any") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        if (nargs >= 2) {
            const char *s = VARVAL_fn(args[1]); if (!s) s = "";
            int slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0 || i1 > slen) { *out = FAILDESCR; return 1; }
            if (i2 <= 0) i2 = slen + 1;
            int p = i1 - 1, end = i2 - 1;
            if (p < 0 || p >= slen || p >= end || !strchr(cv, s[p])) { *out = FAILDESCR; return 1; }
            *out = INTVAL(p + 2); return 1;
        }
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj), p0 = scan_pos - 1;
        if (p0 < 0 || p0 >= slen || !strchr(cv, scan_subj[p0])) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p0 + 2); return 1;
    }
    if (!strcmp(fn,"many") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        if (nargs >= 2) {
            const char *s = VARVAL_fn(args[1]); if (!s) s = "";
            int slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0 || i1 > slen) { *out = FAILDESCR; return 1; }
            if (i2 <= 0) i2 = slen + 1;
            int p = i1 - 1, end = i2 - 1;
            if (p < 0 || p >= slen || p >= end || !strchr(cv, s[p])) { *out = FAILDESCR; return 1; }
            while (p < end && p < slen && strchr(cv, s[p])) p++;
            *out = INTVAL(p + 1); return 1;
        }
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj), p0 = scan_pos - 1;
        if (p0 < 0 || p0 >= slen || !strchr(cv, scan_subj[p0])) { *out = FAILDESCR; return 1; }
        while (p0 < slen && strchr(cv, scan_subj[p0])) p0++;
        *out = INTVAL(p0 + 1); return 1;
    }
    if (!strcmp(fn,"upto") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        if (nargs >= 2) {
            const char *s = VARVAL_fn(args[1]); if (!s) s = "";
            int slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0 || i1 > slen) { *out = FAILDESCR; return 1; }
            if (i2 <= 0) i2 = slen + 1;
            int p = i1 - 1, end = (i2 - 1 < slen ? i2 - 1 : slen);
            while (p < end && !strchr(cv, s[p])) p++;
            if (p >= end) { *out = FAILDESCR; return 1; }
            *out = INTVAL(p + 1); return 1;
        }
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj), p0 = scan_pos - 1;
        while (p0 < slen && !strchr(cv, scan_subj[p0])) p0++;
        if (p0 >= slen) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p0 + 1); return 1;
    }
    if (!strcmp(fn,"tab") && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = GC_malloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"move") && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int n = (int)to_int(args[0]);
        int target = scan_pos + n;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = GC_malloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"match") && nargs >= 1 && scan_pos > 0) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) { *out = FAILDESCR; return 1; }
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int plen = (int)strlen(pat), p0 = scan_pos - 1;
        int slen = (int)strlen(scan_subj);
        if (p0 + plen > slen || strncmp(scan_subj + p0, pat, plen) != 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(scan_pos + plen); return 1;
    }
    if (!strcmp(fn,"bal") && nargs >= 1 && scan_pos > 0) {
        const char *c1; int c1len;
        if (!cset_resolve(args[0], &c1, &c1len)) { *out = FAILDESCR; return 1; }
        const char *c2 = "("; int c2len = 1;
        const char *c3 = ")"; int c3len = 1;
        if (nargs >= 2) { const char *v; int vlen; if (cset_resolve(args[1], &v, &vlen) && vlen > 0) { c2 = v; c2len = vlen; } }
        if (nargs >= 3) { const char *v; int vlen; if (cset_resolve(args[2], &v, &vlen) && vlen > 0) { c3 = v; c3len = vlen; } }
        const char *s = scan_subj ? scan_subj : ""; int slen = (int)strlen(s);
        int p = scan_pos - 1; long long cnt = 0;
        while (p < slen) {
            unsigned char ch = (unsigned char)s[p];
            if (cnt == 0 && cset_has(c1, c1len, ch)) { *out = INTVAL(p + 1); return 1; }
            if (cset_has(c2, c2len, ch)) cnt++;
            else if (cset_has(c3, c3len, ch)) { cnt--; if (cnt < 0) { *out = FAILDESCR; return 1; } }
            p++;
        }
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(fn,"find") && nargs >= 1 && scan_pos > 0) {
        const char *needle = VARVAL_fn(args[0]); if (!needle) { *out = FAILDESCR; return 1; }
        const char *hay    = (nargs >= 2) ? VARVAL_fn(args[1]) : (const char *)0; if (!hay) hay = scan_subj ? scan_subj : "";
        int nlen = (int)strlen(needle), hlen = (int)strlen(hay);
        int start = scan_pos - 1;
        for (int i = start; i + nlen <= hlen; i++) {
            if (strncmp(hay + i, needle, nlen) == 0) { *out = INTVAL(i + 1); return 1; }
        }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"TIME") && nargs <= 1) {
        *out = INTVAL((long long)(clock() * 1000 / CLOCKS_PER_SEC)); return 1;
    }
    if (!strcmp(fn,"DATE") && nargs <= 1) {
        time_t now = time(NULL); struct tm *tm = localtime(&now);
        char *buf = (char *)GC_malloc(64); strftime(buf, 64, "%m/%d/%Y %H:%M:%S", tm);
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn,"SIZE") && nargs == 1) {
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
        if (IS_INT_fn(v)||IS_REAL_fn(v)) { *out = INTVAL(0); return 1; }
        if (IS_CSET_fn(v)) {
            int klen = kw_cset_len(v.s);
            *out = INTVAL(klen >= 0 ? klen : (v.s ? (long)strlen(v.s) : 0));
            return 1;
        }
        const char *s = VARVAL_fn(v); if (!s) { *out = INTVAL(0); return 1; }
        if (strchr(s,'\x01')) {
            long n=1; for(const char *p=s;*p;p++) if(*p=='\x01') n++;
            *out = INTVAL(n); return 1;
        }
        long len = IS_CSET_fn(v) ? (long)strlen(s) : (v.slen > 0 ? v.slen : (long)strlen(s));
        *out = INTVAL(len); return 1;
    }
    if (!strcmp(fn,"NONNULL") && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = FAILDESCR; return 1; }
        if (v.v == DT_S && (!v.s || v.s[0]=='\0')) { *out = FAILDESCR; return 1; }
        *out = v; return 1;
    }
    if (!strcmp(fn,"ICN_CASE_EQ") && nargs == 2) {
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
    if (!strcmp(fn,"ICN_SWAP_TOP2") && nargs == 2) {
        *out = args[0];
        return 1;
    }
    if (!strcmp(fn,"ICN_NULL") && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (v.v == DT_S && (!v.s || v.s[0]=='\0')) { *out = NULVCL; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"insert") && nargs >= 2) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = args[1];
        DESCR_t vd = (nargs >= 3) ? args[2] : NULVCL;
        char kb[64]; const char *ks;
        if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
        else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
        else                     { ks=VARVAL_fn(kd); if(!ks) ks=""; }
        table_set_descr(td.tbl, ks, kd, vd);
        *out = td; return 1;
    }
    if (!strcmp(fn,"delete") && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks;
        if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
        else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
        else                     { ks=VARVAL_fn(kd); if(!ks) ks=""; }
        unsigned h=0x1505;
        { const char *p=ks; while(*p){h=(h<<5)+h^(unsigned char)*p++;} h&=0xFF; }
        TBPAIR_t **pp=&td.tbl->buckets[h];
        while(*pp) {
            if(strcmp((*pp)->key,ks)==0){TBPAIR_t *del=*pp;*pp=del->next;td.tbl->size--;break;}
            pp=&(*pp)->next;
        }
        *out = td; return 1;
    }
    if (!strcmp(fn,"member") && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks;
        if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
        else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
        else                     { ks=VARVAL_fn(kd); if(!ks) ks=""; }
        if (!table_has(td.tbl,ks)) { *out=FAILDESCR; return 1; }
        *out = table_get(td.tbl,ks); return 1;
    }
    if (!strcmp(fn,"key") && nargs == 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T || !td.tbl) { *out=FAILDESCR; return 1; }
        for (int _bi=0;_bi<TABLE_BUCKETS;_bi++)
            if (td.tbl->buckets[_bi]) {
                *out = td.tbl->buckets[_bi]->key_descr; return 1;
            }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn,"push") && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t *nb=GC_malloc((n+1)*sizeof(DESCR_t));
            nb[0]=vd;
            if(old&&n>0) memcpy(nb+1,old,n*sizeof(DESCR_t));
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
        }
        *out = ld; return 1;
    }
    if (!strcmp(fn,"put") && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t *nb=GC_malloc((n+1)*sizeof(DESCR_t));
            if(old&&n>0) memcpy(nb,old,n*sizeof(DESCR_t));
            nb[n]=vd;
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
        }
        *out = ld; return 1;
    }
    if (!strcmp(fn,"get") && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        *out = ret; return 1;
    }
    if (!strcmp(fn,"pop") && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        *out = ret; return 1;
    }
    if (!strcmp(fn,"pull") && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
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
    if ((!strcmp(fn,"sort")&&nargs==1)||(!strcmp(fn,"sortf")&&nargs==2)) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        if (n<=0) { *out=ld; return 1; }
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr) { *out=ld; return 1; }
        DESCR_t *sorted=GC_malloc(n*sizeof(DESCR_t));
        memcpy(sorted,arr,n*sizeof(DESCR_t));
        int field_idx=(!strcmp(fn,"sortf")&&nargs==2)?(int)to_int(args[1])-1:-1;
        for(int _i=1;_i<n;_i++){
            DESCR_t key=sorted[_i]; int _j=_i-1;
            while(_j>=0){
                DESCR_t a=sorted[_j],b=key;
                if(field_idx>=0){
                    if(a.v==DT_DATA&&a.u){DATINST_t*_ia=(DATINST_t*)a.u;if(_ia->type&&field_idx<_ia->type->nfields)a=_ia->fields[field_idx];}
                    if(b.v==DT_DATA&&b.u){DATINST_t*_ib=(DATINST_t*)b.u;if(_ib->type&&field_idx<_ib->type->nfields)b=_ib->fields[field_idx];}
                }
                int cmp;
                if(IS_INT_fn(a)&&IS_INT_fn(b)) cmp=(a.i>b.i)?1:(a.i<b.i)?-1:0;
                else{const char*sa=VARVAL_fn(a),*sb=VARVAL_fn(b);cmp=strcmp(sa?sa:"",sb?sb:"");}
                if(cmp<=0) break;
                sorted[_j+1]=sorted[_j];_j--;
            }
            sorted[_j+1]=key;
        }
        DESCR_t res=ld;
        FIELD_SET_fn(res,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=sorted});
        FIELD_SET_fn(res,"frame_size",INTVAL(n));
        *out=res; return 1;
    }
    if (!strcmp(fn,"FIELD_GET") && nargs == 2) {
        DESCR_t obj  = args[0];
        DESCR_t fname_d = args[1];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
        *out = dat_field_get(fname, obj); return 1;
    }
    if (!strcmp(fn,"FIELD_SET") && nargs == 3) {
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
    if (!strcmp(fn,"MAKELIST")) {
        static int list_reg3 = 0;
        if (!list_reg3) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); list_reg3 = 1; }
        DESCR_t *elems = GC_malloc((nargs>0?nargs:1)*sizeof(DESCR_t));
        for (int _j=0;_j<nargs;_j++) elems[_j]=args[_j];
        DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
        *out = DATCON_fn("list", eptr, INTVAL(nargs), STRVAL("list")); return 1;
    }
    if (!strcmp(fn,"RECORD_REGISTER") && nargs >= 1) {
        extern void record_register(const char *spec);
        const char *spec = VARVAL_fn(args[0]);
        if (spec && *spec) record_register(spec);
        *out = NULVCL; return 1;
    }
    if (!strcmp(fn,"RECORD_MAKE") && nargs >= 1) {
        const char *rname = VARVAL_fn(args[0]);
        if (!rname || !*rname) { *out=FAILDESCR; return 1; }
        DatType *_dt = dat_find_type(rname);
        if (!_dt) { *out=FAILDESCR; return 1; }
        DESCR_t fargs[FRAME_SLOT_MAX];
        int nf = nargs - 1;
        for (int _j=0;_j<nf&&_j<FRAME_SLOT_MAX;_j++) fargs[_j]=args[1+_j];
        *out = dat_construct(_dt, fargs, nf); return 1;
    }
    if (!strcmp(fn,"open") && (nargs == 1 || nargs == 2)) {
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
        if (idx >= 0 && idx < FH_MAX) fh_name[idx] = GC_strdup(path);
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn,"close") && nargs == 1) {
        if (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) {
            int idx = (int)args[0].i;
            FILE *fp = fh_get(idx);
            if (fp && idx > 2) { fclose(fp); fh_free(idx); }
        }
        *out = args[0]; return 1;
    }
    if (!strcmp(fn,"read") && nargs == 1) {
        FILE *fp = (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        char buf[4096];
        if (!fgets(buf, sizeof buf, fp)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        *out = STRVAL(GC_strdup(buf)); return 1;
    }
    if (!strcmp(fn,"reads") && nargs == 2) {
        FILE *fp = (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        int n = (int)to_int(args[1]);
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = GC_malloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, fp);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    if (!strcmp(fn,"IDENTICAL") && nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        int same = (a.v == b.v);
        if (same) {
            if      (a.v == DT_I)               same = (a.i == b.i);
            else if (a.v == DT_R)               same = (a.r == b.r);
            else if (a.v == DT_S || a.v == DT_SNUL)
                same = (a.s == b.s || (a.s && b.s && strcmp(a.s,b.s)==0));
            else                                same = (a.ptr == b.ptr);
        }
        *out = same ? b : FAILDESCR; return 1;
    }
    if (!strcmp(fn,"set") && nargs <= 1) {
        TBBLK_t *tbl = table_new();
        if (nargs == 1 && args[0].v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(args[0], "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s,"list")==0) {
                DESCR_t ea = FIELD_GET_fn(args[0], "frame_elems");
                int n = (int)FIELD_GET_fn(args[0], "frame_size").i;
                DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                if (elems) for (int _i = 0; _i < n; _i++)
                    table_set_descr(tbl, NULL, elems[_i], INTVAL(1));
            }
        }
        *out = TABLE_VAL(tbl); return 1;
    }
    if (!strcmp(fn,"ASGN") && nargs == 2) {
        DESCR_t rhs = args[0];
        if (IS_FAIL_fn(rhs)) { *out = FAILDESCR; return 1; }
        DESCR_t lref = args[1];
        if (lref.v == DT_S && lref.s) NV_SET_fn(lref.s, rhs);
        *out = rhs; return 1;
    }
    if (!strcmp(fn,"variable") && nargs == 1) {
        const char *vname = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!vname) { *out = FAILDESCR; return 1; }
        DESCR_t v = NV_GET_fn(vname);
        *out = IS_FAIL_fn(v) ? FAILDESCR : v; return 1;
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
        int _cmp=strcmp(_ls,_rs); *out=(_cmp op 0)?_r:FAILDESCR; return 1; } while(0)
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
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=a; return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { char *ch=GC_malloc(2); ch[0]=s[0]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
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
            const char *s=NULL;
            if (IS_INT_fn(a)) { char *nb=GC_malloc(32); snprintf(nb,32,"%lld",(long long)a.i); s=nb; }
            else if (IS_REAL_fn(a)) { char *nb=GC_malloc(64); real_str(a.r,nb,64); s=nb; }
            else { s=VARVAL_fn(a); }
            if(!s) s="";
            unsigned char in_set[256]={0}; for(const char *p=s;*p;p++) in_set[(unsigned char)*p]=1;
            char *buf=GC_malloc(256); int n=0;
            for(int c=1;c<256;c++) if(!in_set[c]) buf[n++]=(char)c; buf[n]='\0';
            *out=STRVAL(buf); return 1;
        }
        if (fn[0]=='?' && fn[1]=='\0') {
            if (IS_INT_fn(a)) { *out=(a.i>0)?INTVAL((long long)(rand()%(int)a.i)+1):FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=REALVAL((double)rand()/RAND_MAX*a.r); return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { int n=(int)strlen(s); char *ch=GC_malloc(2); ch[0]=s[rand()%n]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
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
        if (!strcmp(fn,"<"))  _NUMREL(<);
        if (!strcmp(fn,"<=")) _NUMREL(<=);
        if (!strcmp(fn,">"))  _NUMREL(>);
        if (!strcmp(fn,">=")) _NUMREL(>=);
        if (!strcmp(fn,"="))  _NUMREL(==);
        if (!strcmp(fn,"~=")) _NUMREL(!=);
        if (!strcmp(fn,"<<"))  _STRREL(<);
        if (!strcmp(fn,"<<=")) _STRREL(<=);
        if (!strcmp(fn,">>"))  _STRREL(>);
        if (!strcmp(fn,">>=")) _STRREL(>=);
        if (!strcmp(fn,"=="))  _STRREL(==);
        if (!strcmp(fn,"~==")) _STRREL(!=);
        if (!strcmp(fn,"EQ")||!strcmp(fn,"NE")||!strcmp(fn,"LT")||!strcmp(fn,"LE")||!strcmp(fn,"GT")||!strcmp(fn,"GE")) {
            DESCR_t _l=args[0],_r=args[1]; _OPCOERCE(_l); _OPCOERCE(_r);
            double a=IS_REAL_fn(_l)?_l.r:(double)_l.i, b=IS_REAL_fn(_r)?_r.r:(double)_r.i;
            int ok = !strcmp(fn,"EQ")?(a==b):!strcmp(fn,"NE")?(a!=b):!strcmp(fn,"LT")?(a<b)
                   : !strcmp(fn,"LE")?(a<=b):!strcmp(fn,"GT")?(a>b):(a>=b);
            *out = ok ? NULVCL : FAILDESCR; return 1;
        }
        if (!strcmp(fn,"LGT")||!strcmp(fn,"LLT")||!strcmp(fn,"LGE")||!strcmp(fn,"LLE")||!strcmp(fn,"LEQ")||!strcmp(fn,"LNE")) {
            const char *_ls=VARVAL_fn(args[0]); if(!_ls)_ls="";
            const char *_rs=VARVAL_fn(args[1]); if(!_rs)_rs="";
            int c=strcmp(_ls,_rs);
            int ok = !strcmp(fn,"LGT")?(c>0):!strcmp(fn,"LLT")?(c<0):!strcmp(fn,"LGE")?(c>=0)
                   : !strcmp(fn,"LLE")?(c<=0):!strcmp(fn,"LEQ")?(c==0):(c!=0);
            *out = ok ? NULVCL : FAILDESCR; return 1;
        }
        if (!strcmp(fn,"===")) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?r:FAILDESCR; return 1;
        }
        if (!strcmp(fn,"~===")) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?FAILDESCR:r; return 1;
        }
        if (!strcmp(fn,"[]")) {
            *out=subscript_get(l,r); return 1;
        }
        if (!strcmp(fn,"++") || !strcmp(fn,"--") || !strcmp(fn,"**")) {
            char _lbuf[64], _rbuf[64];
            const char *la, *ra;
            if (IS_INT_fn(l))       { snprintf(_lbuf,sizeof _lbuf,"%lld",(long long)l.i); la=_lbuf; }
            else if (IS_REAL_fn(l)) { real_str(l.r,_lbuf,sizeof _lbuf); la=_lbuf; }
            else                    { la=VARVAL_fn(l); if(!la) la=""; }
            if (IS_INT_fn(r))       { snprintf(_rbuf,sizeof _rbuf,"%lld",(long long)r.i); ra=_rbuf; }
            else if (IS_REAL_fn(r)) { real_str(r.r,_rbuf,sizeof _rbuf); ra=_rbuf; }
            else                    { ra=VARVAL_fn(r); if(!ra) ra=""; }
            if (fn[0]=='+') *out=CSETVAL(cset_canonical(cset_union(la,ra)));
            else if (fn[1]=='-') *out=CSETVAL(cset_canonical(cset_diff(la,ra)));
            else *out=CSETVAL(cset_canonical(cset_inter(la,ra)));
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
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
