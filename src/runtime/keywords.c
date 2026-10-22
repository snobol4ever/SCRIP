#include "rt/rt_arena.h"
#include "g_runtime.h"
#include "rt_diag.h"
#include "ct_arena.h"
#include "core.h"
#include "sil_macros.h"
#include "keywords.h"
#include "rt/rt_coexpr.h"
#include "rtx/rtcc.h"
#include "../ir/ab_abi.h"
#include <math.h>
#include <time.h>
#include <unistd.h>
const char *cset_canonical(const char *cs, int len);
void fh_ensure_init(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long *rt_anchor_ptr(void) { return &g_runtime.abi.anchor; }
typedef struct g_kw_cset_ent { const char *ptr; const char *name; int len; unsigned char bits[32]; } kw_cset_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned kw_cset_hash(const char *p) { uint64_t x = (uint64_t)(uintptr_t)p; x ^= x >> 17; x *= 0x9E3779B97F4A7C15ull; x ^= x >> 29; return (unsigned)x; }
static unsigned kw_cset_chash(const char *p, int len) { unsigned h = 5381u; for (int i = 0; i < len; i++) h = h * 33u + (unsigned char)p[i]; return h; }
static int kw_cset_same(const kw_cset_ent_t *e, const char *p, int len) { return e->ptr && e->len == len && !memcmp(e->ptr, p, (size_t)len); }
static void kw_cset_cindex_insert(int idx) {
    const kw_cset_ent_t *e = &g_runtime.kw.kw_cset_names[idx];
    if (!e->ptr || e->len < 0) return;
    int m = g_runtime.kw.kw_cset_hcap - 1;
    for (int sl = (int)(kw_cset_chash(e->ptr, e->len) & (unsigned)m);; sl = (sl + 1) & m) {
        int v = g_runtime.kw.kw_cset_cidx[sl];
        if (!v) { g_runtime.kw.kw_cset_cidx[sl] = idx + 1; return; }
        if (kw_cset_same(&g_runtime.kw.kw_cset_names[v - 1], e->ptr, e->len)) return;
    }
}
static int kw_cset_find_content(const char *p, int len) {
    if (!g_runtime.kw.kw_cset_cidx || !p || len < 0) return -1;
    int m = g_runtime.kw.kw_cset_hcap - 1;
    for (int sl = (int)(kw_cset_chash(p, len) & (unsigned)m);; sl = (sl + 1) & m) {
        int v = g_runtime.kw.kw_cset_cidx[sl];
        if (!v) return -1;
        if (kw_cset_same(&g_runtime.kw.kw_cset_names[v - 1], p, len)) return v - 1;
    }
}
static void kw_cset_hindex_insert(int idx) {
    int m = g_runtime.kw.kw_cset_hcap - 1;
    for (int sl = (int)(kw_cset_hash(g_runtime.kw.kw_cset_names[idx].ptr) & (unsigned)m);; sl = (sl + 1) & m) if (!g_runtime.kw.kw_cset_hidx[sl]) { g_runtime.kw.kw_cset_hidx[sl] = idx + 1; return; }
}
static __attribute__((noinline)) void kw_cset_compact(void) {
    int j = 0;
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].ptr || g_runtime.kw.kw_cset_names[i].name) {
        if (j != i) g_runtime.kw.kw_cset_names[j] = g_runtime.kw.kw_cset_names[i];
        j++;
    }
    if (j == g_runtime.kw.kw_cset_count) return;
    g_runtime.kw.kw_cset_count = j;
    memset(g_runtime.kw.kw_cset_cidx, 0, (size_t)g_runtime.kw.kw_cset_hcap * sizeof(int));
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].ptr) kw_cset_cindex_insert(i);
}
static void kw_cset_hindex_refresh(void) {
    extern long rt_gc_runs_count(void);
    long gen = rt_gc_runs_count();
    if (gen == g_runtime.kw.kw_cset_hidx_gen) return;
    g_runtime.kw.kw_cset_hidx_gen = gen;
    if (!g_runtime.kw.kw_cset_hidx || g_runtime.kw.kw_cset_hcap <= 0) return;
    kw_cset_compact();
    memset(g_runtime.kw.kw_cset_hidx, 0, (size_t)g_runtime.kw.kw_cset_hcap * sizeof(int));
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].ptr) kw_cset_hindex_insert(i);
}
static void kw_cset_hindex_rebuild(void) {
    int want = 64;
    while (want < g_runtime.kw.kw_cset_cap * 2) want <<= 1;
    if (want != g_runtime.kw.kw_cset_hcap) {
        ct_drop(g_runtime.kw.kw_cset_hidx);
        ct_drop(g_runtime.kw.kw_cset_cidx);
        g_runtime.kw.kw_cset_hidx = (int *) ct_zalloc((size_t)want, sizeof(int));
        g_runtime.kw.kw_cset_cidx = (int *) ct_zalloc((size_t)want, sizeof(int));
        g_runtime.kw.kw_cset_hcap = want;
    } else {
        memset(g_runtime.kw.kw_cset_hidx, 0, (size_t)want * sizeof(int));
        memset(g_runtime.kw.kw_cset_cidx, 0, (size_t)want * sizeof(int));
    }
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].ptr) { kw_cset_hindex_insert(i); kw_cset_cindex_insert(i); }
}
static int kw_cset_find_ptr(const char *p) {
    if (!g_runtime.kw.kw_cset_hidx || !p) return -1;
    kw_cset_hindex_refresh();
    int m = g_runtime.kw.kw_cset_hcap - 1;
    for (int sl = (int)(kw_cset_hash(p) & (unsigned)m);; sl = (sl + 1) & m) { int v = g_runtime.kw.kw_cset_hidx[sl]; if (!v) return -1; if (g_runtime.kw.kw_cset_names[v - 1].ptr == p) return v - 1; }
}
static void kw_cset_grow(void) {
    if (g_runtime.kw.kw_cset_count < g_runtime.kw.kw_cset_cap) return;
    g_runtime.kw.kw_cset_cap = g_runtime.kw.kw_cset_cap ? g_runtime.kw.kw_cset_cap * 2 : 16;
    g_runtime.kw.kw_cset_names = (kw_cset_ent_t *) ct_grow(g_runtime.kw.kw_cset_names, (size_t)g_runtime.kw.kw_cset_cap * sizeof(kw_cset_ent_t));
    kw_cset_hindex_rebuild();
}
static void kw_cset_bits_fill(kw_cset_ent_t *e) {
    memset(e->bits, 0, sizeof e->bits);
    if (e->ptr) for (int i = 0; i < e->len; i++) { unsigned c = (unsigned char)e->ptr[i]; e->bits[c >> 3] |= (unsigned char)(1u << (c & 7)); }
}
const unsigned char *kw_cset_bits(const char *ptr) { int hit = kw_cset_find_ptr(ptr); return hit >= 0 ? g_runtime.kw.kw_cset_names[hit].bits : (const unsigned char *)0; }
const unsigned char *rt_icn_cset_bits_d(uint64_t lo, uint64_t hi) {
    uint64_t w[2];
    w[0] = lo;
    w[1] = hi;
    DESCR_t sv;
    memcpy(&sv, w, sizeof sv);
    return (IS_CSET_fn(sv) && sv.s) ? kw_cset_bits(sv.s) : (const unsigned char *)0;
}
static void kw_cset_append(const char *ptr, const char *name, int len) {
    kw_cset_grow();
    g_runtime.kw.kw_cset_names[g_runtime.kw.kw_cset_count].ptr = ptr;
    g_runtime.kw.kw_cset_names[g_runtime.kw.kw_cset_count].name = name;
    g_runtime.kw.kw_cset_names[g_runtime.kw.kw_cset_count].len = len;
    kw_cset_bits_fill(&g_runtime.kw.kw_cset_names[g_runtime.kw.kw_cset_count]);
    g_runtime.kw.kw_cset_count++;
    if (ptr) { kw_cset_hindex_insert(g_runtime.kw.kw_cset_count - 1); kw_cset_cindex_insert(g_runtime.kw.kw_cset_count - 1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t make_kw_cset(const char *chars, const char *kw_name) {
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].name && !strcmp(g_runtime.kw.kw_cset_names[i].name, kw_name))
        return CSETVAL(g_runtime.kw.kw_cset_names[i].ptr);
    const char *arena = cset_canonical(chars, (int)strlen(chars));
    char *stable = rt_heap_strdup_c(arena);
    int clen = (int)strlen(stable);
    kw_cset_append(stable, kw_name, clen);
    return CSETVAL(stable);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kw_cset_reg(const char *chars, const char *name, int len) {
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].name && !strcmp(g_runtime.kw.kw_cset_names[i].name, name)) return;
    extern void *rt_wsb_alloc(size_t);
    char *stable = (char *)rt_wsb_alloc((size_t)len + 1);
    memcpy(stable, chars, (size_t)len);
    stable[len] = '\0';
    kw_cset_append(stable, name, len);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kw_cset_prime(void) {
    static int primed = 0;
    if (primed) return;
    primed = 1;
    kw_cset_reg("abcdefghijklmnopqrstuvwxyz", "&lcase", 26);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "&ucase", 26);
    kw_cset_reg("0123456789", "&digits", 10);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", "&letters", 52);
    { char a[128]; for (int c=0;c<128;c++) a[c]=(char)c; kw_cset_reg(a, "&ascii", 128); }
    { char a[256]; for (int c=0;c<256;c++) a[c]=(char)c; kw_cset_reg(a, "&cset", 256); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kw_cset_slot_of(const char *kw_name) {
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) if (g_runtime.kw.kw_cset_names[i].name && !strcmp(g_runtime.kw.kw_cset_names[i].name, kw_name)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void kw_errtext_gc_root(void) { extern void rt_gc_visit_raw(const char **loc); if (g_runtime.kw.sno_errtext) rt_gc_visit_raw(&g_runtime.kw.sno_errtext); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void kw_cset_gc_roots(void) {
    extern void rt_gc_visit_raw(const char **loc);
    kw_errtext_gc_root();
    if (!g_runtime.kw.kw_cset_names) return;
    rt_gc_visit_raw((const char **)&g_runtime.kw.kw_cset_names);
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) {
        if (!g_runtime.kw.kw_cset_names[i].name) continue;
        if (g_runtime.kw.kw_cset_names[i].ptr) rt_gc_visit_raw(&g_runtime.kw.kw_cset_names[i].ptr);
        rt_gc_visit_raw(&g_runtime.kw.kw_cset_names[i].name);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void kw_cset_gc_weak(void) {
    extern int rt_gc_weak_keep(const char **loc);
    for (int i = 0; i < g_runtime.kw.kw_cset_count; i++) {
        kw_cset_ent_t *e = &g_runtime.kw.kw_cset_names[i];
        if (e->name || !e->ptr || rt_gc_weak_keep(&e->ptr)) continue;
        e->ptr = (const char *)0;
        e->len = -1;
    }
    memset(g_runtime.kw.kw_cset_regc_ptr, 0, sizeof g_runtime.kw.kw_cset_regc_ptr);
}
const char *kw_cset_intern(const char *canon, int len) {
    kw_cset_prime();
    if (!canon) { canon = ""; if (len > 0) len = 0; }
    if (len >= 0) { int hit = kw_cset_find_content(canon, len); if (hit >= 0) return g_runtime.kw.kw_cset_names[hit].ptr; }
    {
        extern void *rt_wsb_alloc(size_t);
        char *stable = (char *)rt_wsb_alloc((size_t)len + 1);
        memcpy(stable, canon, (size_t)len);
        stable[len] = '\0';
        kw_cset_append(stable, NULL, len);
        return stable;
    }
}
void rt_icn_cset_register(const char *ptr, int len) {
    if (!ptr) return;
    {
        unsigned h = (unsigned)(((uintptr_t)ptr >> 4) & 63u);
        if (g_runtime.kw.kw_cset_regc_ptr[h] == ptr && g_runtime.kw.kw_cset_regc_len[h] == len) return;
        g_runtime.kw.kw_cset_regc_ptr[h] = ptr;
        g_runtime.kw.kw_cset_regc_len[h] = len;
    }
    kw_cset_prime();
    {
        int hit = kw_cset_find_ptr(ptr);
        if (hit >= 0) { if (g_runtime.kw.kw_cset_names[hit].len != len) { g_runtime.kw.kw_cset_names[hit].len = len; kw_cset_bits_fill(&g_runtime.kw.kw_cset_names[hit]); } return; }
    }
    kw_cset_append(ptr, NULL, len);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *kw_cset_name(const char *ptr) {
    kw_cset_prime();
    { int hit = kw_cset_find_ptr(ptr); if (hit >= 0 && g_runtime.kw.kw_cset_names[hit].name) return g_runtime.kw.kw_cset_names[hit].name; }
    if (ptr) for (int i = 0; i < g_runtime.kw.kw_cset_count; i++)
        if (g_runtime.kw.kw_cset_names[i].ptr && g_runtime.kw.kw_cset_names[i].ptr[0] != '\0' && g_runtime.kw.kw_cset_names[i].name && !strcmp(g_runtime.kw.kw_cset_names[i].ptr, ptr))
        return g_runtime.kw.kw_cset_names[i].name;
    {
        int plen = kw_cset_len(ptr);
        if (plen > 0) for (int i = 0; i < g_runtime.kw.kw_cset_count; i++)
            if (g_runtime.kw.kw_cset_names[i].name && g_runtime.kw.kw_cset_names[i].len == plen && g_runtime.kw.kw_cset_names[i].ptr && !memcmp(g_runtime.kw.kw_cset_names[i].ptr, ptr, (size_t)plen))
            return g_runtime.kw.kw_cset_names[i].name;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kw_cset_len(const char *ptr) {
    kw_cset_prime();
    { int hit = kw_cset_find_ptr(ptr); if (hit >= 0) return g_runtime.kw.kw_cset_names[hit].len; }
    if (ptr && ptr[0]) { int hit = kw_cset_find_content(ptr, (int)strlen(ptr)); if (hit >= 0) return g_runtime.kw.kw_cset_names[hit].len; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *kw_cset_const_str(const char *kw) {
    if (!kw) return (const char *)0;
    if (kw[0] == '&') kw++;
    if (!strcmp(kw, "lcase")) return "abcdefghijklmnopqrstuvwxyz";
    if (!strcmp(kw, "ucase")) return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(kw, "letters")) return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    if (!strcmp(kw, "digits")) return "0123456789";
    return (const char *)0;
}
#define KWB_INT 1
#define KWB_STR 2
#define KWB_PROT 1
typedef struct { const char *name; unsigned char kind; unsigned char prot; int64_t *cell; int64_t init; const char *sval; const char *sym; } KWB_ENT_t;
static int64_t g_kwb_own[8] = { 0, 0, 0, 0, 0, 0, 0, 1 };
extern int64_t g_rt_kwb_own[8] __attribute__((alias("g_kwb_own")));
static KWB_ENT_t g_kwb[] = { { "ANCHOR", KWB_INT, 0, (int64_t *)&g_runtime.abi.anchor, 0, (const char *)0 , "rt_anchor_g" }, { "TRIM", KWB_INT, 0, &g_runtime.core.kw_trim, 1, (const char *)0 ,
    "g_runtime.core.kw_trim" }, { "CASE", KWB_INT, 0, &g_kwb_own[0], 0, (const char *)0 , "g_rt_kwb_own" }, { "CODE", KWB_INT, 0, &g_runtime.core.kw_code, 0, (const char *)0 ,
    "g_runtime.core.kw_code" }, { "DUMP", KWB_INT, 0, (int64_t *)&g_runtime.abi.dump, 0, (const char *)0 , "g_runtime.abi.dump" }, { "ERRLIMIT", KWB_INT, 0, &g_runtime.abi.kw_errlimit, 0,
    (const char *)0 , "g_kw_errlimit" }, { "ERRTYPE", KWB_INT, 0, &g_kwb_own[1], 0, (const char *)0 , "g_rt_kwb_own" }, { "FTRACE", KWB_INT, 0, &g_runtime.abi.kw_ftrace, 0, (const char *)0 ,
    "g_kw_ftrace" }, { "FULLSCAN", KWB_INT, 0, &g_runtime.core.kw_fullscan, 1, (const char *)0 , "g_runtime.core.kw_fullscan" }, { "MAXLNGTH", KWB_INT, 0, (int64_t *)&g_runtime.abi.maxlngth, 16777216,
    (const char *)0 , "g_runtime.abi.maxlngth" }, { "STLIMIT", KWB_INT, 0, &g_runtime.misc.kw_stlimit, 2147483647, (const char *)0 , "g_kw_stlimit" }, { "TRACE", KWB_INT, 0,
    (int64_t *)&g_runtime.abi.trace, 0, (const char *)0 , "g_runtime.abi.trace" }, { "ABEND", KWB_INT, 0, &g_kwb_own[2], 0, (const char *)0 , "g_rt_kwb_own" }, { "INPUT", KWB_INT, 0, &g_kwb_own[3], 1,
    (const char *)0 , "g_rt_kwb_own" }, { "OUTPUT", KWB_INT, 0, &g_kwb_own[4], 1, (const char *)0 , "g_rt_kwb_own" }, { "PROFILE", KWB_INT, 0, &g_kwb_own[5], 0, (const char *)0 , "g_rt_kwb_own" },
    { "COMPARE", KWB_INT, 0, &g_kwb_own[6], 0, (const char *)0 , "g_rt_kwb_own" }, { "STCOUNT", KWB_INT, KWB_PROT, (int64_t *)&g_runtime.abi.stcount, 0, (const char *)0 , "g_runtime.abi.stcount" },
    { "STNO", KWB_INT, KWB_PROT, (int64_t *)&g_runtime.abi.stno, 0, (const char *)0 , "g_runtime.abi.stno" }, { "LASTNO", KWB_INT, KWB_PROT, (int64_t *)&g_runtime.abi.lastno, 0, (const char *)0 ,
    "g_runtime.abi.lastno" }, { "LINE", KWB_INT, KWB_PROT, (int64_t *)&g_runtime.abi.line, 0, (const char *)0 , "g_runtime.abi.line" }, { "LASTLINE", KWB_INT, KWB_PROT,
    (int64_t *)&g_runtime.abi.lastline, 0, (const char *)0 , "g_runtime.abi.lastline" }, { "FILE", KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 }, { "LASTFILE", KWB_STR,
    KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 }, { "FNCLEVEL", KWB_INT, KWB_PROT, &g_runtime.abi.kw_fnclevel, 0, (const char *)0 , "g_kw_fnclevel" }, { "UCASE", KWB_STR, KWB_PROT,
    (int64_t *)0, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" , (const char *)0 }, { "LCASE", KWB_STR, KWB_PROT, (int64_t *)0, 0, "abcdefghijklmnopqrstuvwxyz" , (const char *)0 }, { "RTNTYPE", KWB_STR, KWB_PROT,
    (int64_t *)0, 0, (const char *)0 , (const char *)0 }, { "ERRTEXT", KWB_STR, 0, (int64_t *)0, 0, (const char *)0 , (const char *)0 }, { "ALPHABET", KWB_STR, KWB_PROT, (int64_t *)0, 0,
    (const char *)0 , (const char *)0 }, { "USER_DECLARED_CONSTANTS", KWB_INT, 0, &g_kwb_own[7], 1, (const char *)0 , "g_rt_kwb_own" }, { "MAXINT", KWB_INT, KWB_PROT, &g_runtime.kw.kw_maxint, 0,
    (const char *)0 , "g_runtime.kw.kw_maxint" }, };
static const int g_kwb_n = (int)(sizeof g_kwb / sizeof g_kwb[0]);
static KWB_ENT_t *g_kwb_bound = g_kwb;
static int g_kwb_bound_n = (int)(sizeof g_kwb / sizeof g_kwb[0]);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_kw_direct_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_KW_DIRECT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_kw_direct_sym(int idx, int *soff, const void **base) {
    if (!rt_kw_direct_on()) return (const char *)0;
    if (idx < 0 || idx >= g_kwb_bound_n || g_kwb_bound != g_kwb) return (const char *)0;
    KWB_ENT_t *e = &g_kwb_bound[idx];
    if (e->kind != KWB_INT || !e->cell || !e->sym) return (const char *)0;
    if ((char *)e->cell >= (char *)&g_runtime && (char *)e->cell < (char *)(&g_runtime + 1)) {
        *soff = (int)((char *)e->cell - (char *)&g_runtime);
        *base = (const void *)&g_runtime;
        return "g_runtime";
    }
    long off = ((char *)e->cell >= (char *)g_kwb_own && (char *)e->cell < (char *)(g_kwb_own + 8)) ? (char *)e->cell - (char *)g_kwb_own : 0;
    *soff = (int)off;
    *base = (const void *)((char *)e->cell - off);
    return e->sym;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_udc_on(void) { return g_kwb_own[7] != 0; }
int rt_kw_input_on(void) { return g_kwb_own[3] != 0; }
int rt_kw_output_on(void) { return g_kwb_own[4] != 0; }
void rt_kw_bind(void *block, int n) { if (block && n > 0) { g_kwb_bound = (KWB_ENT_t *)block; g_kwb_bound_n = n; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kwb_init_once(void) {
    static int done = 0;
    if (done) return;
    done = 1;
    for (int i = 0; i < g_kwb_bound_n; i++) if (g_kwb_bound[i].kind == KWB_INT && !g_kwb_bound[i].prot && g_kwb_bound[i].cell) *g_kwb_bound[i].cell = g_kwb_bound[i].init;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_seed_defaults(void) { kwb_init_once(); { extern void rt_code_atexit_arm(void); rt_code_atexit_arm(); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_kw_spelling_is_canonical(const char *kw) { if (!kw) return 0; if (kw[0] == '&') kw++; for (size_t i = 0; kw[i]; i++) if (kw[i] >= 'a' && kw[i] <= 'z') return 0; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static KWB_ENT_t *kwb_find(const char *kw) {
    if (!kw) return (KWB_ENT_t *)0;
    if (kw[0] == '&') kw++;
    size_t un = strlen(kw);
    char uk[un + 1];
    size_t ui = 0;
    for (; ui < un; ui++) uk[ui] = (kw[ui] >= 'a' && kw[ui] <= 'z') ? (char)(kw[ui] - 'a' + 'A') : kw[ui];
    uk[ui] = '\0';
    kwb_init_once();
    for (int i = 0; i < g_kwb_bound_n; i++) if (!strcmp(g_kwb_bound[i].name, uk)) return &g_kwb_bound[i];
    return (KWB_ENT_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_read_ent(KWB_ENT_t *e, DESCR_t *out) {
    if (!e) return 0;
    if (e->kind == KWB_STR) {
        if (!strcmp(e->name, "RTNTYPE")) { *out = STRVAL(g_runtime.abi.kw_rtntype); return 1; }
        if (!strcmp(e->name, "ERRTEXT")) { *out = STRVAL(g_runtime.kw.sno_errtext ? g_runtime.kw.sno_errtext : ""); return 1; }
        if (!strcmp(e->name, "ALPHABET")) { extern const char alphabet[257]; return (*out = BSTRVAL(alphabet, 256)), 1; }
        if (!strcmp(e->name, "FILE")) { *out = STRVAL(g_runtime.abi.file ? g_runtime.abi.file : ""); return 1; }
        if (!strcmp(e->name, "LASTFILE")) { *out = STRVAL(g_runtime.kw.lastfile ? g_runtime.kw.lastfile : ""); return 1; }
        *out = e->sval ? STRVAL(e->sval) : STRVAL("");
        return 1;
    }
    *out = INTVAL(e->cell ? *e->cell : 0);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_read(const char *kw, DESCR_t *out) { return kwb_read_ent(kwb_find(kw), out); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kwb_error(int code, const char *msg) {
    extern void core_runtime_error(int code, const char *msg);
    extern void core_err_compat_map(int *code, const char **msg);
    core_err_compat_map(&code, &msg);
    extern int core_setexit_armed(void);
    KWB_ENT_t *el = kwb_find("ERRLIMIT");
    if (el && el->cell && *el->cell > 0 && !core_setexit_armed()) {
        *el->cell -= 1;
        KWB_ENT_t *et = kwb_find("ERRTYPE");
        if (et && et->cell) *et->cell = code;
        g_runtime.kw.sno_errtext = msg;
        return 0;
    }
    core_runtime_error(code, msg);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_publish_error(int code, const char *msg) { KWB_ENT_t *et = kwb_find("ERRTYPE"); if (et && et->cell) *et->cell = code; g_runtime.kw.sno_errtext = msg ? rt_heap_strdup_c(msg) : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_publish_error_at_exit(int code, const char *msg) { KWB_ENT_t *et = kwb_find("ERRTYPE"); if (et && et->cell) *et->cell = code; g_runtime.kw.sno_errtext = msg ? msg : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_set_rtntype(int which) {
    static const uint64_t words[4] = { 0x00004E5255544552ull, 0x004E525554455246ull, 0x004E52555445524Eull, 0ull };
    _Static_assert(sizeof(g_runtime.abi.kw_rtntype) >= 8, "rt_kw_set_rtntype stores RETURN / FRETURN / NRETURN as one little-endian 8-byte word");
    *(uint64_t *)(void *)g_runtime.abi.kw_rtntype = words[(which >= 0 && which <= 2) ? which : 3];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_numeric_text(const char *s) {
    if (!s) return 0;
    const char *q = s;
    while (*q == ' ' || *q == '\t' || *q == '\n' || *q == '\r' || *q == '\f' || *q == '\v') q++;
    if (*q == '+' || *q == '-') q++;
    if (q[0] == '0' && (q[1] == 'x' || q[1] == 'X')) return 0;
    char *end = (char *)0;
    double d = strtod(s, &end);
    if (end && end != s && !isfinite(d)) return 0;
    const char *p = (end && end != s) ? (const char *)end : s;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v') p++;
    return *p == '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write_ent(KWB_ENT_t *e, DESCR_t v) {
    if (!e) return 0;
    if (e->kind == KWB_STR && !strcmp(e->name, "ERRTEXT")) { const char *s = VARVAL_fn(v); g_runtime.kw.sno_errtext = rt_heap_strdup_c(s ? s : ""); return 1; }
    int is_num = IS_INT(v) || IS_REAL(v);
    if (!is_num) is_num = kwb_numeric_text(VARVAL_fn(v));
    if (!is_num) return kwb_error(208, "keyword value assigned is not integer") ? 1 : -1;
    int64_t iv = IS_INT(v) ? v.i : (int64_t)to_real(v);
    if (strcmp(e->name, "STLIMIT") && (iv < 0 || iv > 16777216 || (!strcmp(e->name, "PROFILE") && iv > 2))) return kwb_error(210, "keyword value assigned is negative or too large") ? 1 : -1;
    if (!strcmp(e->name, "MAXLNGTH") && iv < 1024) return kwb_error(287, "value assigned to keyword maxlngth is too small") ? 1 : -1;
    if (e->prot) return kwb_error(209, "keyword in assignment is protected") ? 1 : -1;
    if (e->cell) *e->cell = iv;
    if (!strcmp(e->name, "ERRTYPE")) return kwb_error((int)iv, g_runtime.kw.sno_errtext ? g_runtime.kw.sno_errtext : "") ? 1 : -1;
    if (!strcmp(e->name, "DUMP") && iv) { extern void rt_dump_atexit_arm(void); rt_dump_atexit_arm(); }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write(const char *kw, DESCR_t v) { return kwb_write_ent(kwb_find(kw), v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_dump_values(void (*emit)(void *cx, const char *name, DESCR_t v), void *cx) {
    static const char names[][9] = { "ANCHOR", "CASE", "CODE", "DUMP", "ERRLIMIT", "ERRTEXT", "ERRTYPE", "FILE", "FNCLEVEL", "FTRACE", "FULLSCAN", "INPUT", "LASTFILE", "LASTLINE", "LASTNO", "LINE",
        "MAXLNGTH", "OUTPUT", "PROFILE", "RTNTYPE", "STCOUNT", "STLIMIT", "STNO", "TRACE", "TRIM"};
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); i++) { DESCR_t v; if (kwb_read(names[i], &v)) emit(cx, names[i], v); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_kw_index(const char *kw) { if (!kw) return -1; if (!sn4_kw_spelling_is_canonical(kw)) return -1; KWB_ENT_t *e = kwb_find(kw); if (!e) return -1; return (int)(e - g_kwb_bound); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_kw_read_idx(int64_t idx) { if (idx < 0 || idx >= (int64_t)g_kwb_bound_n) return NULVCL; kwb_init_once(); DESCR_t out; if (kwb_read_ent(&g_kwb_bound[idx], &out)) return out; return NULVCL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_kw_write_idx(int64_t idx, DESCR_t v) { if (idx < 0 || idx >= (int64_t)g_kwb_bound_n) return v; kwb_init_once(); return kwb_write_ent(&g_kwb_bound[idx], v) < 0 ? FAILDESCR : v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t kw_read(const char *kw) {
    if (!kw) return FAILDESCR;
    if (!strcmp(kw,"pos")) return INTVAL(g_runtime.abi.scan_pos);
    if (!strcmp(kw,"subject")) {
        extern long rt_scan_subj_len(void);
        if (!g_runtime.abi.scan_subj) return STRVAL("");
        { long n = rt_scan_subj_len(); return (n >= 0) ? BSTRVAL((char *)g_runtime.abi.scan_subj, n) : STRVAL(g_runtime.abi.scan_subj); }
    }
    if (!strcmp(kw,"e")) return REALVAL(2.718281828459045);
    if (!strcmp(kw,"pi")) return REALVAL(3.141592653589793);
    if (!strcmp(kw,"phi")) return REALVAL(1.618033988749895);
    if (!strcmp(kw,"lcase")) return make_kw_cset("abcdefghijklmnopqrstuvwxyz","&lcase");
    if (!strcmp(kw,"ucase")) return make_kw_cset("ABCDEFGHIJKLMNOPQRSTUVWXYZ","&ucase");
    if (!strcmp(kw,"letters")) return make_kw_cset("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz","&letters");
    if (!strcmp(kw,"digits")) return make_kw_cset("0123456789","&digits");
    if (!strcmp(kw,"alphabet")) return kw_read("cset");
    if (!strcmp(kw,"ascii")) {
        static int csi = -1;
        if (csi < 0) csi = kw_cset_slot_of("&ascii");
        if (csi < 0) {
            char ascii_str[128];
            for (int c=0;c<128;c++) ascii_str[c]=(char)c;
            extern void *rt_wsb_alloc(size_t);
            char *stable = (char *)rt_wsb_alloc(129);
            memcpy(stable, ascii_str, 128);
            stable[128] = '\0';
            kw_cset_append(stable, "&ascii", 128);
            csi = g_runtime.kw.kw_cset_count - 1;
        }
        return CSETVAL(g_runtime.kw.kw_cset_names[csi].ptr);
    }
    if (!strcmp(kw,"cset")) {
        static int csi = -1;
        if (csi < 0) csi = kw_cset_slot_of("&cset");
        if (csi < 0) {
            char cset_str[256];
            for (int c=0;c<256;c++) cset_str[c]=(char)c;
            extern void *rt_wsb_alloc(size_t);
            char *stable = (char *)rt_wsb_alloc(257);
            memcpy(stable, cset_str, 256);
            stable[256] = '\0';
            kw_cset_append(stable, "&cset", 256);
            csi = g_runtime.kw.kw_cset_count - 1;
        }
        return CSETVAL(g_runtime.kw.kw_cset_names[csi].ptr);
    }
    {
        if (!strcmp(kw,"errornumber")) { if (!g_runtime.misc.icn_err_valid) return FAILDESCR; return INTVAL(g_runtime.misc.icn_errnumber); }
        if (!strcmp(kw,"errortext")) { if (!g_runtime.misc.icn_err_valid) return FAILDESCR; return STRVAL(rt_heap_strdup_c(g_runtime.misc.icn_errtext ? g_runtime.misc.icn_errtext : "")); }
        if (!strcmp(kw,"errorvalue")) { if (!g_runtime.misc.icn_err_valid) return FAILDESCR; return g_runtime.misc.icn_errvalue; }
        if (!strcmp(kw,"control")) return FAILDESCR;
    }
    {
        if (!strcmp(kw,"error")) return INTVAL(g_runtime.abi.error);
        if (!strcmp(kw,"trace")) return INTVAL(g_runtime.abi.trace);
        if (!strcmp(kw,"dump")) return INTVAL(g_runtime.abi.dump);
        if (!strcmp(kw,"random")) return INTVAL(g_runtime.abi.random);
    }
    {
        if (!strcmp(kw,"anchor")) return INTVAL(g_runtime.abi.anchor);
        if (!strcmp(kw,"maxlngth")) return INTVAL(g_runtime.abi.maxlngth);
        if (!strcmp(kw,"fullscan")) return INTVAL(0);
        if (!strcmp(kw,"stlimit")) return INTVAL(-1);
    }
    {
        if (!strcmp(kw,"stno")) return INTVAL(g_runtime.abi.stno);
        if (!strcmp(kw,"stcount")) return INTVAL(g_runtime.abi.stcount);
        if (!strcmp(kw,"lastno")) return INTVAL(g_runtime.abi.lastno);
        if (!strcmp(kw,"line")) return INTVAL(g_runtime.abi.line);
        if (!strcmp(kw,"lastline")) return INTVAL(g_runtime.abi.lastline);
    }
    { if (!strcmp(kw,"file")) return STRVAL(g_runtime.abi.file ? g_runtime.abi.file : ""); if (!strcmp(kw,"lastfile")) return STRVAL(g_runtime.kw.lastfile ? g_runtime.kw.lastfile : ""); }
    if (!strcmp(kw,"col")) return FAILDESCR;
    if (!strcmp(kw,"row")) return FAILDESCR;
    if (!strcmp(kw,"x")) return FAILDESCR;
    if (!strcmp(kw,"y")) return FAILDESCR;
    { if (!strcmp(kw,"level")) return INTVAL(g_runtime.abi.rt_k_level); }
    if (!strcmp(kw,"lpress")) return INTVAL(-1);
    if (!strcmp(kw,"mpress")) return INTVAL(-2);
    if (!strcmp(kw,"rpress")) return INTVAL(-3);
    if (!strcmp(kw,"lrelease")) return INTVAL(-4);
    if (!strcmp(kw,"mrelease")) return INTVAL(-5);
    if (!strcmp(kw,"rrelease")) return INTVAL(-6);
    if (!strcmp(kw,"ldrag")) return INTVAL(-7);
    if (!strcmp(kw,"mdrag")) return INTVAL(-8);
    if (!strcmp(kw,"rdrag")) return INTVAL(-9);
    if (!strcmp(kw,"resize")) return INTVAL(-10);
    if (!strcmp(kw,"null")) return NULVCL;
    if (!strcmp(kw,"fail")) return FAILDESCR;
    if (!strcmp(kw,"window")) return FAILDESCR;
    if (!strcmp(kw,"input")) { fh_ensure_init(); return FHVAL(0); }
    if (!strcmp(kw,"output")) { fh_ensure_init(); return FHVAL(1); }
    if (!strcmp(kw,"errout")) { fh_ensure_init(); return FHVAL(2); }
    if (!strcmp(kw,"current")) { scrip_coctx_t *cur = g_scrip_co_current ? g_scrip_co_current : scrip_co_gc_root(); DESCR_t d = {0}; d.v = DT_CO; d.p = cur; return d; }
    if (!strcmp(kw,"main")) { DESCR_t d = {0}; d.v = DT_CO; d.p = scrip_co_gc_root(); return d; }
    if (!strcmp(kw,"source")) {
        scrip_coctx_t *cur = g_scrip_co_current ? g_scrip_co_current : scrip_co_gc_root();
        scrip_coctx_t *src = cur->activator ? cur->activator : scrip_co_gc_root();
        DESCR_t d = {0};
        d.v = DT_CO;
        d.p = src;
        return d;
    }
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        extern void *rt_wsb_alloc(size_t);
        if (!strcmp(kw,"date")) { char *buf = rt_wsb_alloc(16); snprintf(buf,16,"%04d/%02d/%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday); return STRVAL(buf); }
        if (!strcmp(kw,"dateline")) {
            static const char *day[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
            static const char *month[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
            int hour = tm->tm_hour;
            const char *merid = (hour >= 12) ? "pm" : "am";
            if (hour > 12) hour -= 12;
            else if (hour < 1) hour += 12;
            char *buf = rt_wsb_alloc(64);
            snprintf(buf,64,"%s, %s %d, %d  %d:%02d %s",day[tm->tm_wday],month[tm->tm_mon],tm->tm_mday,tm->tm_year+1900,hour,tm->tm_min,merid);
            return STRVAL(buf);
        }
        if (!strcmp(kw,"clock")) { char *buf = rt_wsb_alloc(16); snprintf(buf,16,"%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec); return STRVAL(buf); }
        if (!strcmp(kw,"time")) { return INTVAL((int64_t)(clock()*1000/CLOCKS_PER_SEC)); }
    }
    if (!strcmp(kw,"progname")) { extern const char *rt_main_progname(void); return STRVAL(rt_main_progname()); }
    if (!strcmp(kw,"version")) return STRVAL("Icon Version 9.5.25a, September 7, 2025");
    if (!strcmp(kw,"host")) { static char hbuf[256]; if (!hbuf[0]) { if (gethostname(hbuf,sizeof(hbuf)-1) != 0 || !hbuf[0]) strcpy(hbuf,"scrip"); } return STRVAL(hbuf); }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_read(const char *sval) {
    if (!sval) return NULVCL;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    size_t ln = strlen(kw);
    char lk[ln + 1];
    size_t li = 0;
    for (; li < ln; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    DESCR_t kv = kw_read(lk);
    if (!IS_FAIL(kv)) return kv;
    if (!strcmp(lk,"control") || !strcmp(lk,"errornumber") || !strcmp(lk,"errortext") || !strcmp(lk,"errorvalue") || !strcmp(lk,"fail") || !strcmp(lk,"interval") || !strcmp(lk,"meta") ||
        !strcmp(lk,"shift") || !strcmp(lk,"col") || !strcmp(lk,"row") || !strcmp(lk,"x") || !strcmp(lk,"y") || !strcmp(lk,"window")) return FAILDESCR;
    return NV_GET_fn(sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_read_snobol4(const char *sval) {
    if (!sval) return NULVCL;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    size_t ln = strlen(kw);
    char lk[ln + 1];
    size_t li = 0;
    for (; li < ln; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    const int canon = sn4_kw_spelling_is_canonical(kw);
    { DESCR_t bv; if (canon && kwb_read(lk, &bv)) return bv; }
    if (canon && !strcmp(lk, "digits")) return STRVAL("0123456789");
    if (canon && !strcmp(lk, "ht")) return STRVAL("\t");
    if (canon && (!strcmp(lk, "lf") || !strcmp(lk, "nl"))) return STRVAL("\n");
    if (canon && !strcmp(lk, "vt")) return STRVAL("\x0B");
    if (canon && !strcmp(lk, "ff")) return STRVAL("\x0C");
    if (canon && !strcmp(lk, "cr")) return STRVAL("\r");
    if (canon && !strcmp(lk, "esc")) return STRVAL("\x1B");
    size_t bl = strlen(sval);
    char kb[bl + 2];
    const char *ck = sval;
    if (sval[0] != '&') { kb[0] = '&'; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }
    if (rt_udc_on() && NV_CONST_ASSIGNED_fn(ck)) return NV_KW_GET_fn(ck);
    DESCR_t kv = canon ? kw_read(lk) : FAILDESCR;
    if (!IS_FAIL(kv)) return kv;
    if (canon && (!strcmp(lk,"arb") || !strcmp(lk,"bal") || !strcmp(lk,"rem") || !strcmp(lk,"fail") || !strcmp(lk,"fence") || !strcmp(lk,"abort") || !strcmp(lk,"succeed"))) {
        const char *bn = sval[0] == '&' ? sval + 1 : sval;
        return NV_GET_fn(bn);
    }
    if (!rt_udc_on()) {
        const char *en = sval[0] == '&' ? sval : lk;
        char eb[fmt_len("keyword operand is not name of defined keyword: %s", en)];
        snprintf(eb, sizeof eb, "keyword operand is not name of defined keyword: %s", en);
        core_runtime_error(251, eb);
        return FAILDESCR;
    }
    {
        if (!NV_CONST_ASSIGNED_fn(ck)) {
            char eb[fmt_len("&constant read before its one-time assignment: %s", ck)];
            snprintf(eb, sizeof eb, "&constant read before its one-time assignment: %s", ck);
            core_runtime_error(342, eb);
            return FAILDESCR;
        }
        return NV_KW_GET_fn(ck);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_stmt_file_init(const char *file) { g_runtime.abi.file = file; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_stmt_seed_code_fragment_statement_hooks(void) { setenv("SCRIP_SNO_STMTKW", "1", 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_stmt_enter(long stno, long line) {
    g_runtime.abi.lastno = g_runtime.abi.stno;
    g_runtime.abi.stno = stno;
    g_runtime.abi.lastline = g_runtime.abi.line;
    g_runtime.abi.line = line;
    g_runtime.kw.lastfile = (g_runtime.abi.stcount > 0) ? g_runtime.abi.file : (const char *)0;
    if (g_runtime.misc.kw_stlimit < 0) return;
    g_runtime.abi.stcount++;
    if (g_runtime.abi.stcount == 1) rt_stmt_seed_code_fragment_statement_hooks();
#if RT_DIAG
    { extern void rt_trace_keyword_write(const char *, int64_t, long long); rt_trace_keyword_write("STCOUNT", (int64_t)g_runtime.abi.stcount, (long long)stno); }
#endif
    if (g_runtime.misc.kw_stlimit > 0 && g_runtime.abi.stcount > g_runtime.misc.kw_stlimit) kwb_error(244, "statement count exceeds value of stlimit keyword");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_stmt_enter_p(long stno, long line, trace_pend_t *pend) {
    g_runtime.abi.lastno = g_runtime.abi.stno;
    g_runtime.abi.stno = stno;
    g_runtime.abi.lastline = g_runtime.abi.line;
    g_runtime.abi.line = line;
    g_runtime.kw.lastfile = (g_runtime.abi.stcount > 0) ? g_runtime.abi.file : (const char *)0;
    if (g_runtime.misc.kw_stlimit < 0) return;
    g_runtime.abi.stcount++;
    if (g_runtime.abi.stcount == 1) rt_stmt_seed_code_fragment_statement_hooks();
#if RT_DIAG
    { extern void rt_trace_keyword_write_p(const char *, int64_t, long long, trace_pend_t *); rt_trace_keyword_write_p("STCOUNT", (int64_t)g_runtime.abi.stcount, (long long)stno, pend); }
#endif
    if (g_runtime.misc.kw_stlimit > 0 && g_runtime.abi.stcount > g_runtime.misc.kw_stlimit) kwb_error(244, "statement count exceeds value of stlimit keyword");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_gen(const char *sval, long idx) {
    if (!sval) return FAILDESCR;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    if (!strcmp(kw,"features")) {
        static const char *const feats[] = { "UNIX", "ASCII", "co-expressions", "dynamic loading", "environment variables", "external values", "keyboard functions", "large integers", "pipes",
            "system function" };
        int n = (int)(sizeof(feats) / sizeof(feats[0]));
        if (idx < 0 || idx >= n) return FAILDESCR;
        return STRVAL(feats[idx]);
    }
    if (!strcmp(kw,"allocated")) {
        extern long rt_gc_alloc_total(void), rt_gc_alloc_str(void);
        if (idx == 0) return INTVAL(rt_gc_alloc_total());
        if (idx == 1) return INTVAL(0);
        if (idx == 2) return INTVAL(rt_gc_alloc_str());
        if (idx == 3) return INTVAL(rt_gc_alloc_total() - rt_gc_alloc_str());
        return FAILDESCR;
    }
    if (!strcmp(kw,"regions")) { if (idx == 0) return INTVAL(0); if (idx == 1 || idx == 2) return INTVAL(500000); return FAILDESCR; }
    if (!strcmp(kw,"storage")) { extern long rt_gc_bytes_in_use(void); if (idx == 0 || idx == 1) return INTVAL(0); if (idx == 2) return INTVAL(rt_gc_bytes_in_use()); return FAILDESCR; }
    if (!strcmp(kw,"collections")) {
        extern long rt_gc_runs_count(void);
        long g = rt_gc_runs_count();
        if (idx == 0) return INTVAL(g);
        if (idx == 1) return INTVAL(0);
        if (idx == 2) return INTVAL(0);
        if (idx == 3) return INTVAL(0);
        return FAILDESCR;
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_keyword_write_snobol4(const char *sval, DESCR_t v) {
    if (!sval) return 1;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    size_t ln = strlen(kw);
    char lk[ln + 1];
    size_t li = 0;
    for (; li < ln; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    const int canon = sn4_kw_spelling_is_canonical(kw);
    { int r = canon ? kwb_write(lk, v) : 0; if (r) return r > 0; }
    long iv = 0;
    if (IS_INT(v)) iv = (long)v.i;
    else if (IS_REAL(v)) iv = (long)v.r;
    else { const char *s2 = VARVAL_fn(v); if (s2) iv = strtol(s2, (char **)0, 10); }
    if (canon && !strcmp(lk,"error")) { g_runtime.abi.error = iv; return 1; }
    if (canon && !strcmp(lk,"random")) { g_runtime.abi.random = iv; g_runtime.misc.bb_rnd_seed = (unsigned long)iv; return 1; }
    {
        size_t bl = strlen(sval);
        char kb[bl + 2];
        const char *ck = sval;
        if (sval[0] != '&') { kb[0] = '&'; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }
        if (!rt_udc_on()) {
            char eb[fmt_len("keyword operand is not name of defined keyword: %s", ck)];
            snprintf(eb, sizeof eb, "keyword operand is not name of defined keyword: %s", ck);
            core_runtime_error(251, eb);
            return 1;
        }
        NV_KW_SET_fn(ck, v);
    }
    return 1;
}
