#include "rt/rt_arena.h"
#include "core.h"
#include "sil_macros.h"
#include "keywords.h"
#include "rtx/rtcc.h"
#include <time.h>
#include <unistd.h>
const char  *cset_canonical(const char *cs);
extern const char  *scan_subj;
extern int           scan_pos;
extern unsigned long bb_rnd_seed;
void         fh_ensure_init(void);
long g_error  = 0;
long g_trace  = 0;
long g_dump   = 0;
long g_random = 0;
static long g_anchor = 0;
extern long rt_anchor_g __attribute__((alias("g_anchor")));
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long *rt_anchor_ptr(void) { return &g_anchor; }
long g_maxlngth = 5000000;
int  g_jcon   = 0;
long g_stno    = 0;
long g_stcount = 0;
long g_lastno  = 0;
long g_line    = 0;
long g_lastline = 0;
/* ⭐ &FILE/&LASTFILE globals — Lon's grant, 2026-08-27 in-chat to CEO (relayed to kw-missing-4's
   task LEDGER): "I already granted permission for those two globals... &FILE and &LASTFILE will be
   implemented as global variables." Scope: exactly these two cells, parallel to g_line/g_lastline. */
const char *g_file = NULL;
const char *g_lastfile = NULL;
const char *g_sno_errtext = NULL;
typedef struct { const char *ptr; const char *name; int len; } kw_cset_ent_t;
static kw_cset_ent_t *g_kw_cset_names = NULL;
static int g_kw_cset_count = 0;
static int g_kw_cset_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kw_cset_grow(void) {
    if (g_kw_cset_count < g_kw_cset_cap) return;
    g_kw_cset_cap = g_kw_cset_cap ? g_kw_cset_cap * 2 : 16;
    g_kw_cset_names = (kw_cset_ent_t *) realloc(g_kw_cset_names, (size_t)g_kw_cset_cap * sizeof(kw_cset_ent_t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t make_kw_cset(const char *chars, const char *kw_name) {
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].name && !strcmp(g_kw_cset_names[i].name, kw_name))
            return CSETVAL(g_kw_cset_names[i].ptr);
    const char *arena = cset_canonical(chars);
    char *stable = rt_ws_strdup(arena);
    int clen = (int)strlen(stable);
    kw_cset_grow();
    g_kw_cset_names[g_kw_cset_count].ptr  = stable;
    g_kw_cset_names[g_kw_cset_count].name = kw_name;
    g_kw_cset_names[g_kw_cset_count].len  = clen;
    g_kw_cset_count++;
    return CSETVAL(stable);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔ NOT cset_canonical(chars) (icon-ascii-cset-keywords-built-off-by-one): cset_canonical scans its input
   with `for (...; *p; p++)`, so it truncates to "" the moment `chars` contains a byte-0 member -- exactly
   what a correctly-built &ascii/&cset now does at index 0. All 6 callers of kw_cset_reg already pass
   sorted, deduped, complete input (the very thing cset_canonical would compute), so canonicalizing here
   was always redundant work; routing around it with an explicit-length copy costs nothing for the other
   5 and fixes the 6th. This does not touch cset_canonical itself -- its own general-literal gap
   (FINDING-2026-08-27-seat09-icon-cset-embedded-nul-is-a-four-layer-representation-gap.md) is untouched,
   deliberately, same reasoning as that FINDING's own "not fixed this session". */
static void kw_cset_reg(const char *chars, const char *name, int len) {
    for (int i = 0; i < g_kw_cset_count; i++) if (g_kw_cset_names[i].name && !strcmp(g_kw_cset_names[i].name, name)) return;
    kw_cset_grow();
    extern void *rt_ws_alloc(size_t);
    char *stable = (char *)rt_ws_alloc((size_t)len + 1);
    memcpy(stable, chars, (size_t)len);
    stable[len] = '\0';
    g_kw_cset_names[g_kw_cset_count].ptr  = stable;
    g_kw_cset_names[g_kw_cset_count].name = name;
    g_kw_cset_names[g_kw_cset_count].len  = len;
    g_kw_cset_count++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kw_cset_prime(void) {
    static int primed = 0; if (primed) return; primed = 1;
    kw_cset_reg("abcdefghijklmnopqrstuvwxyz", "&lcase", 26);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "&ucase", 26);
    kw_cset_reg("0123456789", "&digits", 10);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", "&letters", 52);
    { static char a[128]; for (int c=0;c<128;c++) a[c]=(char)c; kw_cset_reg(a, "&ascii", 128); }
    { static char a[256]; for (int c=0;c<256;c++) a[c]=(char)c; kw_cset_reg(a, "&cset", 256); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* icn-cset-embedded-nul-four-layer-gap, layer 5 (registry side): a literal charset's TRUE length
   (which can exceed what strlen() reports when the set contains byte 0) is baked by codegen as a
   compile-time constant and registered here, keyed by pointer identity -- the same lookup
   c_rt_size_d/kw_cset_len already perform for the 6 keyword csets, now open to any literal.
   Dedup is by pointer, never by content (strcmp cannot safely compare two embedded-NUL strings). */
void rt_icn_cset_register(const char *ptr, int len) {
    if (!ptr) return;
    kw_cset_prime();
    for (int i = 0; i < g_kw_cset_count; i++) if (g_kw_cset_names[i].ptr == ptr) return;
    kw_cset_grow();
    g_kw_cset_names[g_kw_cset_count].ptr  = ptr;
    g_kw_cset_names[g_kw_cset_count].name = NULL;
    g_kw_cset_names[g_kw_cset_count].len  = len;
    g_kw_cset_count++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *kw_cset_name(const char *ptr) {
    kw_cset_prime();
    /* icn-cset-embedded-nul-four-layer-gap follow-on: &lcase/&ucase/&digits/&letters fold to a
       compile-time IR_LIT_CHARSET at lower_icon.c's lc_key (their content is statically known), so
       their runtime value is a CODEGEN-BAKED buffer, never make_kw_cset's own pointer -- and
       rt_icn_cset_register now registers every IR_LIT_CHARSET literal, keyword-folded ones included,
       with name=NULL (it has no way to know a given literal happens to BE a keyword's folded value).
       An unnamed entry can never supply a name, so it must not win the exact-pointer match ahead of
       the real keyword entry (registered separately, at a different address, by kw_cset_prime/
       make_kw_cset) -- without this guard the unnamed hit short-circuits both loops and image()/etc.
       print the raw character content instead of "&lcase". kw_cset_len is unaffected: it does not
       care which entry it matches, since the length agrees either way. */
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr && g_kw_cset_names[i].name) return g_kw_cset_names[i].name;
    /* NOT strcmp against an entry starting with byte 0 (icon-ascii-cset-keywords-built-off-by-one):
       same unsafe-comparison shape as kw_cset_len's own fallback, just above -- see its comment. An
       unrelated short/empty pointer (e.g. rt_cset_compl's empty-complement result) would otherwise
       strcmp-"match" a correctly-built &ascii/&cset and print its name instead of the real content. */
    if (ptr) for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr && g_kw_cset_names[i].ptr[0] != '\0' && g_kw_cset_names[i].name && !strcmp(g_kw_cset_names[i].ptr, ptr)) return g_kw_cset_names[i].name;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kw_cset_len(const char *ptr) {
    kw_cset_prime();
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr) return g_kw_cset_names[i].len;
    /* NOT strcmp against an entry whose OWN content starts with byte 0 (icon-ascii-cset-keywords-built-off-by-one):
       strcmp has no length for `ptr` to check, so it can only compare up to the shorter side's own NUL -- an
       entry that starts with chr(0) (a correctly-built &ascii/&cset) matches ANY other pointer that also starts
       with chr(0), including a short/empty/unrelated one, as spuriously "equal". Such an entry is reachable only
       by the exact-pointer match above, which is the normal case for every caller that holds the real registered
       pointer; refusing the content fallback here is strictly safer than a wrong length. */
    if (ptr) for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr && g_kw_cset_names[i].ptr[0] != '\0' && !strcmp(g_kw_cset_names[i].ptr, ptr)) return g_kw_cset_names[i].len;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *kw_cset_const_str(const char *kw) {
    if (!kw) return (const char *)0;
    if (kw[0] == '&') kw++;
    if (!strcmp(kw, "lcase"))   return "abcdefghijklmnopqrstuvwxyz";
    if (!strcmp(kw, "ucase"))   return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(kw, "letters")) return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    if (!strcmp(kw, "digits"))  return "0123456789";
    return (const char *)0;
}
#define KWB_INT 1
#define KWB_STR 2
#define KWB_PROT 1
typedef struct { const char *name; unsigned char kind; unsigned char prot; int64_t *cell; int64_t init; const char *sval; const char *sym; } KWB_ENT_t;
static int64_t kwb_own[8] = { 0, 0, 0, 0, 0, 0, 0, 1 };
extern int64_t rt_kwb_own[8] __attribute__((alias("kwb_own")));
static KWB_ENT_t g_kwb[] = {
    { "ANCHOR",   KWB_INT, 0, (int64_t *)&g_anchor,    0,          (const char *)0 , "rt_anchor_g" },
    { "TRIM",     KWB_INT, 0, &kw_trim,                1,          (const char *)0 , "kw_trim" },
    { "CASE",     KWB_INT, 0, &kwb_own[0],             0,          (const char *)0 , "rt_kwb_own" },
    { "CODE",     KWB_INT, 0, &kw_code,                0,          (const char *)0 , "kw_code" },
    { "DUMP",     KWB_INT, 0, (int64_t *)&g_dump,      0,          (const char *)0 , "g_dump" },
    { "ERRLIMIT", KWB_INT, 0, &kw_errlimit,            0,          (const char *)0 , "kw_errlimit" },
    { "ERRTYPE",  KWB_INT, 0, &kwb_own[1],             0,          (const char *)0 , "rt_kwb_own" },
    { "FTRACE",   KWB_INT, 0, &kw_ftrace,              0,          (const char *)0 , "kw_ftrace" },
    { "FULLSCAN", KWB_INT, 0, &kw_fullscan,            1,          (const char *)0 , "kw_fullscan" },
    { "MAXLNGTH", KWB_INT, 0, (int64_t *)&g_maxlngth,  16777216,   (const char *)0 , "g_maxlngth" },
    { "STLIMIT",  KWB_INT, 0, &kw_stlimit,             2147483647, (const char *)0 , "kw_stlimit" },
    { "TRACE",    KWB_INT, 0, (int64_t *)&g_trace,     0,          (const char *)0 , "g_trace" },
    { "ABEND",    KWB_INT, 0, &kwb_own[2],             0,          (const char *)0 , "rt_kwb_own" },
    { "INPUT",    KWB_INT, 0, &kwb_own[3],             1,          (const char *)0 , "rt_kwb_own" },
    { "OUTPUT",   KWB_INT, 0, &kwb_own[4],             1,          (const char *)0 , "rt_kwb_own" },
    { "PROFILE",  KWB_INT, 0, &kwb_own[5],             0,          (const char *)0 , "rt_kwb_own" },
    { "COMPARE",  KWB_INT, 0, &kwb_own[6],             0,          (const char *)0 , "rt_kwb_own" },
    { "STCOUNT",  KWB_INT, KWB_PROT, (int64_t *)&g_stcount,  0, (const char *)0 , "g_stcount" },
    { "STNO",     KWB_INT, KWB_PROT, (int64_t *)&g_stno,     0, (const char *)0 , "g_stno" },
    { "LASTNO",   KWB_INT, KWB_PROT, (int64_t *)&g_lastno,   0, (const char *)0 , "g_lastno" },
    { "LINE",     KWB_INT, KWB_PROT, (int64_t *)&g_line,     0, (const char *)0 , "g_line" },
    { "LASTLINE", KWB_INT, KWB_PROT, (int64_t *)&g_lastline, 0, (const char *)0 , "g_lastline" },
    { "FILE",     KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 },
    { "LASTFILE", KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 },
    { "FNCLEVEL", KWB_INT, KWB_PROT, &kw_fnclevel,           0, (const char *)0 , "kw_fnclevel" },
    { "UCASE",    KWB_STR, KWB_PROT, (int64_t *)0, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" , (const char *)0 },
    { "LCASE",    KWB_STR, KWB_PROT, (int64_t *)0, 0, "abcdefghijklmnopqrstuvwxyz" , (const char *)0 },
    { "RTNTYPE",  KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 },
    { "ERRTEXT",  KWB_STR, 0,        (int64_t *)0, 0, (const char *)0 , (const char *)0 },
    { "ALPHABET", KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 , (const char *)0 },
    { "USER_DECLARED_CONSTANTS", KWB_INT, 0, &kwb_own[7], 1, (const char *)0 , "rt_kwb_own" },
};
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
    long off = ((char *)e->cell >= (char *)kwb_own && (char *)e->cell < (char *)(kwb_own + 8)) ? (char *)e->cell - (char *)kwb_own : 0;
    *soff = (int)off; *base = (const void *)((char *)e->cell - off);
    return e->sym;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_udc_on(void) { return kwb_own[7] != 0; }
void rt_kw_bind(void *block, int n) { if (block && n > 0) { g_kwb_bound = (KWB_ENT_t *)block; g_kwb_bound_n = n; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kwb_init_once(void) {
    static int done = 0; if (done) return; done = 1;
    for (int i = 0; i < g_kwb_bound_n; i++) if (g_kwb_bound[i].kind == KWB_INT && !g_kwb_bound[i].prot && g_kwb_bound[i].cell) *g_kwb_bound[i].cell = g_kwb_bound[i].init;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_seed_defaults(void) { kwb_init_once(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static KWB_ENT_t *kwb_find(const char *kw) {
    if (!kw) return (KWB_ENT_t *)0;
    if (kw[0] == '&') kw++;
    char uk[64]; size_t ui = 0;
    for (; kw[ui] && ui < sizeof(uk) - 1; ui++) uk[ui] = (kw[ui] >= 'a' && kw[ui] <= 'z') ? (char)(kw[ui] - 'a' + 'A') : kw[ui];
    uk[ui] = '\0';
    kwb_init_once();
    for (int i = 0; i < g_kwb_bound_n; i++) if (!strcmp(g_kwb_bound[i].name, uk)) return &g_kwb_bound[i];
    return (KWB_ENT_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_read_ent(KWB_ENT_t *e, DESCR_t *out) {
    if (!e) return 0;
    if (e->kind == KWB_STR) {
        if (!strcmp(e->name, "RTNTYPE")) { *out = STRVAL(kw_rtntype); return 1; }
        if (!strcmp(e->name, "ERRTEXT")) { *out = STRVAL(g_sno_errtext ? g_sno_errtext : ""); return 1; }
        if (!strcmp(e->name, "ALPHABET")) { extern char alphabet[257]; return (*out = BSTRVAL(alphabet, 256)), 1; }
        if (!strcmp(e->name, "FILE"))     { extern const char *g_file;     *out = STRVAL(g_file ? g_file : ""); return 1; }
        if (!strcmp(e->name, "LASTFILE")) { extern const char *g_lastfile; *out = STRVAL(g_lastfile ? g_lastfile : ""); return 1; }
        *out = e->sval ? STRVAL(e->sval) : STRVAL(""); return 1;
    }
    *out = INTVAL(e->cell ? *e->cell : 0); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_read(const char *kw, DESCR_t *out) { return kwb_read_ent(kwb_find(kw), out); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kwb_error(int code, const char *msg) {
    extern void core_runtime_error(int code, const char *msg);
    KWB_ENT_t *el = kwb_find("ERRLIMIT");
    if (el && el->cell && *el->cell > 0) { *el->cell -= 1; KWB_ENT_t *et = kwb_find("ERRTYPE"); if (et && et->cell) *et->cell = code; g_sno_errtext = msg; return 0; }
    core_runtime_error(code, msg); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_publish_error(int code, const char *msg) {
    KWB_ENT_t *et = kwb_find("ERRTYPE"); if (et && et->cell) *et->cell = code;
    g_sno_errtext = msg ? rt_ws_strdup_c(msg) : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_set_rtntype(int which) {
    static const char *names[3] = { "RETURN", "FRETURN", "NRETURN" };
    const char *s = (which >= 0 && which <= 2) ? names[which] : "";
    size_t n = strlen(s); if (n > sizeof(kw_rtntype) - 1) n = sizeof(kw_rtntype) - 1;
    memcpy(kw_rtntype, s, n); kw_rtntype[n] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_set_rtntype_role(int role) {
    extern int rt_g_ret_by_name;
    rt_kw_set_rtntype(role == 2 ? 1 : (rt_g_ret_by_name ? 2 : 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write_ent(KWB_ENT_t *e, DESCR_t v) {
    if (!e) return 0;
    if (e->kind == KWB_STR && !strcmp(e->name, "ERRTEXT")) { const char *s = VARVAL_fn(v); g_sno_errtext = rt_ws_strdup_c(s ? s : ""); return 1; }
    int is_num = IS_INT(v) || IS_REAL(v);
    if (!is_num) { const char *s = VARVAL_fn(v); char *end = (char *)0; if (s && *s) { (void)strtol(s, &end, 10); is_num = (end && *end == '\0'); } }
    if (!is_num) return kwb_error(208, "keyword value assigned is not integer") ? 1 : -1;
    int64_t iv = IS_INT(v) ? v.i : (int64_t)to_real(v);
    if (strcmp(e->name, "STLIMIT") && (iv < 0 || iv > 16777216 || (!strcmp(e->name, "PROFILE") && iv > 2))) return kwb_error(210, "keyword value assigned is negative or too large") ? 1 : -1;
    if (!strcmp(e->name, "MAXLNGTH") && iv < 1024) return kwb_error(287, "value assigned to keyword maxlngth is too small") ? 1 : -1;
    if (e->prot) return kwb_error(209, "keyword in assignment is protected") ? 1 : -1;
    if (e->cell) *e->cell = iv;
    if (!strcmp(e->name, "ERRTYPE")) return kwb_error((int)iv, g_sno_errtext ? g_sno_errtext : "") ? 1 : -1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write(const char *kw, DESCR_t v) { return kwb_write_ent(kwb_find(kw), v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_kw_index(const char *kw) {
    if (!kw) return -1;
    KWB_ENT_t *e = kwb_find(kw); if (!e) return -1;
    return (int)(e - g_kwb_bound);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_kw_read_idx(int64_t idx) {
    if (idx < 0 || idx >= (int64_t)g_kwb_bound_n) return NULVCL;
    kwb_init_once();
    DESCR_t out; if (kwb_read_ent(&g_kwb_bound[idx], &out)) return out;
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_kw_write_idx(int64_t idx, DESCR_t v) {
    if (idx < 0 || idx >= (int64_t)g_kwb_bound_n) return v;
    kwb_init_once();
    return kwb_write_ent(&g_kwb_bound[idx], v) < 0 ? FAILDESCR : v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t kw_read(const char *kw) {
    if (!kw) return FAILDESCR;
    if (!strcmp(kw,"pos"))     return INTVAL(scan_pos);
    if (!strcmp(kw,"subject")) return scan_subj ? STRVAL(scan_subj) : STRVAL("");
    if (!strcmp(kw,"e"))   return REALVAL(2.718281828459045);
    if (!strcmp(kw,"pi"))  return REALVAL(3.141592653589793);
    if (!strcmp(kw,"phi")) return REALVAL(1.618033988749895);
    if (!strcmp(kw,"lcase"))   return make_kw_cset("abcdefghijklmnopqrstuvwxyz","&lcase");
    if (!strcmp(kw,"ucase"))   return make_kw_cset("ABCDEFGHIJKLMNOPQRSTUVWXYZ","&ucase");
    if (!strcmp(kw,"letters")) return make_kw_cset("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz","&letters");
    if (!strcmp(kw,"digits"))  return make_kw_cset("0123456789","&digits");
    if (!strcmp(kw,"alphabet")) return kw_read("cset");
    if (!strcmp(kw,"ascii")) {
        static const char *cs = NULL;
        if (!cs) {
            static char ascii_str[128];
            for (int c=0;c<128;c++) ascii_str[c]=(char)c;
            /* NOT cset_canonical() -- see kw_cset_reg's comment (icon-ascii-cset-keywords-built-off-by-one):
               it truncates to "" on a leading byte-0, and ascii_str is already sorted/deduped/complete. */
            extern void *rt_ws_alloc(size_t);
            char *stable = (char *)rt_ws_alloc(129); memcpy(stable, ascii_str, 128); stable[128] = '\0';
            kw_cset_grow();
            g_kw_cset_names[g_kw_cset_count].ptr  = stable;
            g_kw_cset_names[g_kw_cset_count].name = "&ascii";
            g_kw_cset_names[g_kw_cset_count].len  = 128;
            g_kw_cset_count++;
            cs = stable;
        }
        return CSETVAL(cs);
    }
    if (!strcmp(kw,"cset")) {
        static const char *cs = NULL;
        if (!cs) {
            static char cset_str[256];
            for (int c=0;c<256;c++) cset_str[c]=(char)c;
            /* NOT cset_canonical() -- see kw_cset_reg's comment (icon-ascii-cset-keywords-built-off-by-one):
               it truncates to "" on a leading byte-0, and cset_str is already sorted/deduped/complete. */
            extern void *rt_ws_alloc(size_t);
            char *stable = (char *)rt_ws_alloc(257); memcpy(stable, cset_str, 256); stable[256] = '\0';
            kw_cset_grow();
            g_kw_cset_names[g_kw_cset_count].ptr  = stable;
            g_kw_cset_names[g_kw_cset_count].name = "&cset";
            g_kw_cset_names[g_kw_cset_count].len  = 256;
            g_kw_cset_count++;
            cs = stable;
        }
        return CSETVAL(cs);
    }
    { extern long g_icn_errnumber; extern const char *g_icn_errtext; extern DESCR_t g_icn_errvalue; extern int g_icn_err_valid;
      if (!strcmp(kw,"errornumber")) { if (!g_icn_err_valid) return FAILDESCR; return INTVAL(g_icn_errnumber); }
      if (!strcmp(kw,"errortext"))   { if (!g_icn_err_valid) return FAILDESCR; DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S; d.s = rt_ws_strdup_c(g_icn_errtext ? g_icn_errtext : ""); return d; }
      if (!strcmp(kw,"errorvalue"))  { if (!g_icn_err_valid) return FAILDESCR; return g_icn_errvalue; }
      if (!strcmp(kw,"control"))     return FAILDESCR; }
    { extern long g_error, g_trace, g_dump, g_random;
      if (!strcmp(kw,"error"))  return INTVAL(g_error);
      if (!strcmp(kw,"trace"))  return INTVAL(g_trace);
      if (!strcmp(kw,"dump"))   return INTVAL(g_dump);
      if (!strcmp(kw,"random")) return INTVAL(g_random);
    }
    { extern long g_maxlngth;
      if (!strcmp(kw,"anchor"))   return INTVAL(g_anchor);
      if (!strcmp(kw,"maxlngth")) return INTVAL(g_maxlngth);
      if (!strcmp(kw,"fullscan")) return INTVAL(0);
      if (!strcmp(kw,"stlimit"))  return INTVAL(-1);
    }
    { extern long g_stno, g_stcount, g_lastno, g_line, g_lastline;
      if (!strcmp(kw,"stno"))     return INTVAL(g_stno);
      if (!strcmp(kw,"stcount"))  return INTVAL(g_stcount);
      if (!strcmp(kw,"lastno"))   return INTVAL(g_lastno);
      if (!strcmp(kw,"line"))     return INTVAL(g_line);
      if (!strcmp(kw,"lastline")) return INTVAL(g_lastline);
    }
    { extern const char *g_file, *g_lastfile;
      if (!strcmp(kw,"file"))     return STRVAL(g_file ? g_file : "");
      if (!strcmp(kw,"lastfile")) return STRVAL(g_lastfile ? g_lastfile : "");
    }
    if (!strcmp(kw,"col"))     return INTVAL(0);
    if (!strcmp(kw,"row"))     return INTVAL(0);
    if (!strcmp(kw,"x"))       return INTVAL(0);
    if (!strcmp(kw,"y"))       return INTVAL(0);
    { extern int rt_k_level; if (!strcmp(kw,"level")) return INTVAL(rt_k_level); }
    if (!strcmp(kw,"lpress"))   return INTVAL(-1);
    if (!strcmp(kw,"mpress"))   return INTVAL(-2);
    if (!strcmp(kw,"rpress"))   return INTVAL(-3);
    if (!strcmp(kw,"lrelease")) return INTVAL(-4);
    if (!strcmp(kw,"mrelease")) return INTVAL(-5);
    if (!strcmp(kw,"rrelease")) return INTVAL(-6);
    if (!strcmp(kw,"ldrag"))    return INTVAL(-7);
    if (!strcmp(kw,"mdrag"))    return INTVAL(-8);
    if (!strcmp(kw,"rdrag"))    return INTVAL(-9);
    if (!strcmp(kw,"resize"))   return INTVAL(-10);
    if (!strcmp(kw,"null"))    return NULVCL;
    if (!strcmp(kw,"fail"))    return FAILDESCR;
    if (!strcmp(kw,"window"))  return NULVCL;
    if (!strcmp(kw,"input"))   { fh_ensure_init(); return FHVAL(0); }
    if (!strcmp(kw,"output"))  { fh_ensure_init(); return FHVAL(1); }
    if (!strcmp(kw,"errout"))  { fh_ensure_init(); return FHVAL(2); }
    if (!strcmp(kw,"current")) return STRVAL("co-expression_1(0)");
    if (!strcmp(kw,"main"))    return STRVAL("co-expression_1(0)");
    if (!strcmp(kw,"source"))  return STRVAL("co-expression_1(0)");
    { time_t t = time(NULL); struct tm *tm = localtime(&t);
      if (!strcmp(kw,"date")) {
          char *buf = rt_ws_alloc(16);
          snprintf(buf,16,"%04d/%02d/%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"dateline")) {
          char *buf = rt_ws_alloc(64);
          strftime(buf,64,"%A, %B %e, %Y  %l:%M %P",tm);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"clock")) {
          char *buf = rt_ws_alloc(16);
          snprintf(buf,16,"%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"time")) {
          return INTVAL((int64_t)(clock()*1000/CLOCKS_PER_SEC));
      }
    }
    if (!strcmp(kw,"version")) return STRVAL("Jcon Version 2.2");
    if (!strcmp(kw,"host")) { static char hbuf[256]; if (!hbuf[0]) { if (gethostname(hbuf,sizeof(hbuf)-1) != 0 || !hbuf[0]) strcpy(hbuf,"scrip"); } return STRVAL(hbuf); }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_read(const char *sval) {
    if (!sval) return NULVCL;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    char lk[64]; size_t li = 0;
    for (; kw[li] && li < sizeof(lk) - 1; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    DESCR_t kv = kw_read(lk);
    if (!IS_FAIL(kv)) return kv;
    if (!strcmp(lk,"control") || !strcmp(lk,"errornumber") || !strcmp(lk,"errortext") || !strcmp(lk,"errorvalue") || !strcmp(lk,"fail") || !strcmp(lk,"interval") || !strcmp(lk,"meta") || !strcmp(lk,"shift")) return FAILDESCR;
    return NV_GET_fn(sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_read_snobol4(const char *sval) {
    if (!sval) return NULVCL;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    char lk[64]; size_t li = 0;
    for (; kw[li] && li < sizeof(lk) - 1; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    { DESCR_t bv; if (kwb_read(lk, &bv)) return bv; }
    if (!strcmp(lk, "digits")) return STRVAL("0123456789");
    if (!strcmp(lk, "ht")) return STRVAL("\t");
    if (!strcmp(lk, "lf") || !strcmp(lk, "nl")) return STRVAL("\n");
    if (!strcmp(lk, "vt")) return STRVAL("\x0B");
    if (!strcmp(lk, "ff")) return STRVAL("\x0C");
    if (!strcmp(lk, "cr")) return STRVAL("\r");
    if (!strcmp(lk, "esc")) return STRVAL("\x1B");
    char kb[128]; const char *ck = sval; if (sval[0] != '&') { kb[0] = '&'; size_t bl = strlen(sval); if (bl > 126) bl = 126; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }
    if (rt_udc_on() && NV_CONST_ASSIGNED_fn(ck)) return NV_KW_GET_fn(ck);
    DESCR_t kv = kw_read(lk);
    if (!IS_FAIL(kv)) return kv;
    if (!strcmp(lk,"arb") || !strcmp(lk,"bal") || !strcmp(lk,"rem") || !strcmp(lk,"fail") || !strcmp(lk,"fence") || !strcmp(lk,"abort") || !strcmp(lk,"succeed")) { const char *bn = sval[0] == '&' ? sval + 1 : sval; return NV_GET_fn(bn); }
    if (!rt_udc_on()) { char eb[192]; snprintf(eb, sizeof eb, "keyword operand is not name of defined keyword: %s", sval[0] == '&' ? sval : lk); core_runtime_error(251, eb); return NULVCL; }
    {
      if (!NV_CONST_ASSIGNED_fn(ck)) { char eb[192]; snprintf(eb, sizeof eb, "&constant read before its one-time assignment: %s", ck); core_runtime_error(342, eb); return NULVCL; }
      return NV_KW_GET_fn(ck); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ &FILE never changes mid-run (single-file execution, no -INCLUDE support in scope here) --
   set ONCE by rt_stmt_file_init, below, not mirrored every statement the way g_line is. &LASTFILE
   DOES need a per-statement mirror, but NOT g_line's unconditional one: verified against the live
   oracle (/home/resources/x64/bin/sbl -bf) that &LASTFILE reads as the NULL STRING when queried
   from the program's first statement (no previous statement exists yet), not as the current file --
   g_stcount (already incremented past 0 by every statement after the first) is the exact signal
   rt_stmt_enter already carries for "has a previous statement actually run". */
void rt_stmt_file_init(const char *file) { g_file = file; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_stmt_enter(long stno, long line) {
    g_lastno = g_stno;
    g_stno = stno;
    g_lastline = g_line;
    g_line = line;
    g_lastfile = (g_stcount > 0) ? g_file : (const char *)0;
    g_stcount++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_gen(const char *sval, long idx) {
    if (!sval) return FAILDESCR;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    if (!strcmp(kw,"features")) {
        static const char *feats[] = { "UNIX", "Java", "ASCII", "co-expressions", "dynamic loading", "environment variables", "large integers", "pipes", "system function", "graphics" };
        int n = (int)(sizeof(feats) / sizeof(feats[0]));
        if (idx < 0 || idx >= n) return FAILDESCR;
        return STRVAL(feats[idx]);
    }
    if (!strcmp(kw,"allocated")) {
        if (idx >= 0 && idx <= 3) return INTVAL(0);
        return FAILDESCR;
    }
    if (!strcmp(kw,"regions")) {
        if (idx >= 0 && idx <= 2) return INTVAL(0);
        return FAILDESCR;
    }
    if (!strcmp(kw,"storage")) {
        if (idx >= 0 && idx <= 2) return INTVAL(0);
        return FAILDESCR;
    }
    if (!strcmp(kw,"collections")) {
        extern long rt_gc_runs_count(void); long g = rt_gc_runs_count();
        if (idx == 0) return INTVAL(g);
        if (idx == 1) return INTVAL(0);
        if (idx == 2) return INTVAL(g);
        if (idx == 3) return INTVAL(0);
        return FAILDESCR;
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_keyword_write_snobol4(const char *sval, DESCR_t v) {
    if (!sval) return 1;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    char lk[64]; size_t li = 0;
    for (; kw[li] && li < sizeof(lk) - 1; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    { int r = kwb_write(lk, v); if (r) return r > 0; }
    long iv = 0;
    if (IS_INT(v)) iv = (long)v.i;
    else if (IS_REAL(v)) iv = (long)v.r;
    else { const char *s2 = VARVAL_fn(v); if (s2) iv = strtol(s2, (char **)0, 10); }
    if (!strcmp(lk,"error"))    { g_error = iv; return 1; }
    if (!strcmp(lk,"random"))   { g_random = iv; bb_rnd_seed = (unsigned long)iv; return 1; }
    { char kb[128]; const char *ck = sval; if (sval[0] != '&') { kb[0] = '&'; size_t bl = strlen(sval); if (bl > 126) bl = 126; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }
      if (!rt_udc_on()) { char eb[192]; snprintf(eb, sizeof eb, "keyword operand is not name of defined keyword: %s", ck); core_runtime_error(251, eb); return 1; }
      NV_KW_SET_fn(ck, v); }
    return 1;
}
