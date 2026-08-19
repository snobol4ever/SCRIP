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
static long g_anchor = 0;   /* ZW-4 LANDED: keywords.c-private; SNAPSHOT-AT-BEGIN semantics of record land with ZW-1's anchor_snapshot frame cell (SPITBOL manual: &ANCHOR is obtained only at the start of the match) */
extern long rt_anchor_g __attribute__((alias("g_anchor")));   /* ZW-4: the ONE exported name mode-4 TEXT references ([rip + rt_anchor_g]); the SPD-2 blob and bb_match_begin both spell it via the x86() sealed-lea form */
long *rt_anchor_ptr(void) { return &g_anchor; }   /* ZW-4: sole canonical C accessor; BINARY arms embed its return as an absolute movabs */
long g_trim   = 0;
long g_maxlngth = 5000000;
int  g_jcon   = 0;
long g_stno    = 0;
long g_stcount = 0;
long g_lastno  = 0;
const char *g_sno_errtext = NULL;
#define KW_CSET_MAX 16
static struct { const char *ptr; const char *name; int len; } g_kw_cset_names[KW_CSET_MAX];
static int g_kw_cset_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t make_kw_cset(const char *chars, const char *kw_name) {
    for (int i = 0; i < g_kw_cset_count; i++)
        if (!strcmp(g_kw_cset_names[i].name, kw_name))
            return CSETVAL(g_kw_cset_names[i].ptr);
    const char *arena = cset_canonical(chars);
    char *stable = rt_ws_strdup(arena);
    int clen = (int)strlen(stable);
    if (g_kw_cset_count < KW_CSET_MAX) {
        g_kw_cset_names[g_kw_cset_count].ptr  = stable;
        g_kw_cset_names[g_kw_cset_count].name = kw_name;
        g_kw_cset_names[g_kw_cset_count].len  = clen;
        g_kw_cset_count++;
    }
    return CSETVAL(stable);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kw_cset_reg(const char *chars, const char *name, int len) {
    for (int i = 0; i < g_kw_cset_count; i++) if (!strcmp(g_kw_cset_names[i].name, name)) return;
    if (g_kw_cset_count >= KW_CSET_MAX) return;
    g_kw_cset_names[g_kw_cset_count].ptr  = rt_ws_strdup(cset_canonical(chars));
    g_kw_cset_names[g_kw_cset_count].name = name;
    g_kw_cset_names[g_kw_cset_count].len  = len;
    g_kw_cset_count++;
}
static void kw_cset_prime(void) {
    static int primed = 0; if (primed) return; primed = 1;
    kw_cset_reg("abcdefghijklmnopqrstuvwxyz", "&lcase", 26);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "&ucase", 26);
    kw_cset_reg("0123456789", "&digits", 10);
    kw_cset_reg("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", "&letters", 52);
    { static char a[128]; for (int c=1;c<128;c++) a[c-1]=(char)c; a[127]='\0'; kw_cset_reg(a, "&ascii", 128); }
    { static char a[256]; for (int c=1;c<256;c++) a[c-1]=(char)c; a[255]='\0'; kw_cset_reg(a, "&cset", 256); }
}
const char *kw_cset_name(const char *ptr) {
    kw_cset_prime();
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr) return g_kw_cset_names[i].name;
    if (ptr) for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr && !strcmp(g_kw_cset_names[i].ptr, ptr)) return g_kw_cset_names[i].name;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kw_cset_len(const char *ptr) {
    kw_cset_prime();
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr) return g_kw_cset_names[i].len;
    if (ptr) for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr && !strcmp(g_kw_cset_names[i].ptr, ptr)) return g_kw_cset_names[i].len;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*========================================================================================================================================================================================================*/
/* KW-STATIC BLOCK (KW-2, GOAL-SNOBOL4-100 D-3; Lon's in-chat grant 2026-08-19 covers these keyword cells). ONE AUTHORITY for every SNOBOL4 keyword fact: name, kind, protected bit, oracle-true initial */
/* value, and the address of the LIVE cell each consumer already reads. Homing at the consumer's cell -- not a fresh copy -- is what retires the spelled-twice disease measured in FINDING s146 §6: the */
/* block does not add a third spelling of &TRIM, it makes the ONE surviving spelling be the cell core.c's input path actually tests. KW-3 relocates these cells into the emitted program's .data and */
/* repoints `cell`; nothing else has to move. Killswitch SCRIP_KW_STATIC, DEFAULT OFF => the default arm is byte-identical by construction. Initial values are oracle-measured (probe/kw/kw_defaults.ref), */
/* NOT copied from the C initializers, which FINDING s146 §3 measured wrong in six places and self-contradictory in one (&MAXLNGTH: g_maxlngth 5000000 vs kw_maxlngth 524288 vs oracle 16777216). */
#define KWB_INT 1
#define KWB_STR 2
#define KWB_PROT 1
typedef struct { const char *name; unsigned char kind; unsigned char prot; int64_t *cell; int64_t init; const char *sval; } KWB_ENT_t;
static int64_t kwb_own[8];
static KWB_ENT_t g_kwb[] = {
    { "ANCHOR",   KWB_INT, 0, (int64_t *)&g_anchor,    0,          (const char *)0 },
    { "TRIM",     KWB_INT, 0, &kw_trim,                1,          (const char *)0 },
    { "CASE",     KWB_INT, 0, &kwb_own[0],             1,          (const char *)0 },
    { "CODE",     KWB_INT, 0, &kw_code,                0,          (const char *)0 },
    { "DUMP",     KWB_INT, 0, (int64_t *)&g_dump,      0,          (const char *)0 },
    { "ERRLIMIT", KWB_INT, 0, &kw_errlimit,            0,          (const char *)0 },
    { "ERRTYPE",  KWB_INT, 0, &kwb_own[1],             0,          (const char *)0 },
    { "FTRACE",   KWB_INT, 0, &kw_ftrace,              0,          (const char *)0 },
    { "FULLSCAN", KWB_INT, 0, &kw_fullscan,            1,          (const char *)0 },
    { "MAXLNGTH", KWB_INT, 0, (int64_t *)&g_maxlngth,  16777216,   (const char *)0 },
    { "STLIMIT",  KWB_INT, 0, &kw_stlimit,             2147483647, (const char *)0 },
    { "TRACE",    KWB_INT, 0, (int64_t *)&g_trace,     0,          (const char *)0 },
    { "ABEND",    KWB_INT, 0, &kwb_own[2],             0,          (const char *)0 },
    { "INPUT",    KWB_INT, 0, &kwb_own[3],             1,          (const char *)0 },
    { "OUTPUT",   KWB_INT, 0, &kwb_own[4],             1,          (const char *)0 },
    { "PROFILE",  KWB_INT, 0, &kwb_own[5],             0,          (const char *)0 },
    { "COMPARE",  KWB_INT, 0, &kwb_own[6],             0,          (const char *)0 },
    { "STCOUNT",  KWB_INT, KWB_PROT, (int64_t *)&g_stcount,  0, (const char *)0 },
    { "STNO",     KWB_INT, KWB_PROT, (int64_t *)&g_stno,     0, (const char *)0 },
    { "LASTNO",   KWB_INT, KWB_PROT, (int64_t *)&g_lastno,   0, (const char *)0 },
    { "FNCLEVEL", KWB_INT, KWB_PROT, &kw_fnclevel,           0, (const char *)0 },
    { "UCASE",    KWB_STR, KWB_PROT, (int64_t *)0, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" },   /* CLASS C: SNOBOL4 has NO cset datatype -- oracle says STRING/26. The legacy path reached Icon's csets through the SHARED kw_read table; the block gives SNOBOL4 its own home and leaves kw_read untouched for Icon (same carve-out discipline as R-3(g)(d)). */
    { "LCASE",    KWB_STR, KWB_PROT, (int64_t *)0, 0, "abcdefghijklmnopqrstuvwxyz" },
    { "RTNTYPE",  KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 },
    { "ERRTEXT",  KWB_STR, 0,        (int64_t *)0, 0, (const char *)0 },
    { "ALPHABET", KWB_STR, KWB_PROT, (int64_t *)0, 0, (const char *)0 },   /* KW-2 subsumes the core.c:2213 bare-ALPHABET hijack that s145 gated behind SCRIP_SEED_NAMES: the &-form now has ONE home here, and the bare form is an ordinary variable like every other bare name. */
};
static const int g_kwb_n = (int)(sizeof g_kwb / sizeof g_kwb[0]);
static KWB_ENT_t *g_kwb_bound = g_kwb;   /* KW-3 SEAM: the emitted program will bind its own .data block here via rt_kw_bind(); until then the runtime-resident table IS the block, so read/write already go THROUGH the bound pointer and KW-3 becomes a pure relocation of storage. */
static int g_kwb_bound_n = (int)(sizeof g_kwb / sizeof g_kwb[0]);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_kw_static_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_KW_STATIC"); v = (e && *e == '1') ? 1 : 0; } return v; }   /* KW-STATIC killswitch (KW-2, s146). DEFAULT OFF = legacy behaviour byte-identical. =1 arms the block: oracle-true initial values, SNOBOL4-owned string keywords, protection enforcement, and the bare-name special-case family bypassed. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_bind(void *block, int n) { if (block && n > 0) { g_kwb_bound = (KWB_ENT_t *)block; g_kwb_bound_n = n; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void kwb_init_once(void) {
    static int done = 0; if (done) return; done = 1;
    for (int i = 0; i < g_kwb_bound_n; i++) if (g_kwb_bound[i].kind == KWB_INT && !g_kwb_bound[i].prot && g_kwb_bound[i].cell) *g_kwb_bound[i].cell = g_kwb_bound[i].init;
}   /* ⛔ PROTECTED ENTRIES ARE NEVER INITIALIZED HERE, AND THE REASON IS A MEASURED REGRESSION: seeding runs LAZILY at the first keyword touch, not at program start, so &STNO/&STCOUNT/&LASTNO/&FNCLEVEL have ALREADY BEEN COUNTING by then -- writing `init` over them rewinds the live counter. crosscheck/keywords/082_keyword_stcount went PASS->FAIL on exactly this in the s146 by-set A/B. The `init` field describes the SETTABLE knobs' start-of-program configuration; a runtime-maintained repository owns its own value and the block only reads it. */
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
        if (!strcmp(e->name, "RTNTYPE")) { *out = STRVAL(kw_rtntype); return 1; }   /* LIVE strings: &RTNTYPE and &ERRTEXT are protected repositories whose contents the runtime rewrites as it goes, so the block names them but reads the live cell rather than a frozen initializer. */
        if (!strcmp(e->name, "ERRTEXT")) { *out = STRVAL(g_sno_errtext ? g_sno_errtext : ""); return 1; }
        if (!strcmp(e->name, "ALPHABET")) { extern char alphabet[257]; return (*out = BSTRVAL(alphabet, 256)), 1; }
        *out = e->sval ? STRVAL(e->sval) : STRVAL(""); return 1;
    }
    *out = INTVAL(e->cell ? *e->cell : 0); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_read(const char *kw, DESCR_t *out) { return kwb_read_ent(kwb_find(kw), out); }   /* KW-3: the name-keyed entry is now a THIN WRAPPER over the entry-keyed reader, so the by-index fast path (rt_kw_read_idx) and the by-name path cannot answer differently -- they execute the same body. Splitting here rather than duplicating the STR/INT arms is the s68/s70 spelled-twice law applied to KW-3's own seam. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write_ent(KWB_ENT_t *e, DESCR_t v) {
    if (!e) return 0;
    extern void core_runtime_error(int code, const char *msg);
    int is_num = IS_INT(v) || IS_REAL(v);
    if (!is_num) { const char *s = VARVAL_fn(v); char *end = (char *)0; if (s && *s) { (void)strtol(s, &end, 10); is_num = (end && *end == '\0'); } }
    if (!is_num) { core_runtime_error(208, "keyword value assigned is not integer"); return 1; }   /* ⛔ ORDER IS OSCILLOSCOPE-MEASURED, NOT ASSUMED (probe/kw/kw_protected_write.ref): `&ALPHABET = 'x'` raises 208, NOT 209 -- the not-an-integer test fires BEFORE the protected test. Swap these two and the witness goes oracle-wrong. */
    if (e->prot) { core_runtime_error(209, "keyword in assignment is protected"); return 1; }
    if (e->cell) *e->cell = IS_INT(v) ? v.i : (int64_t)to_real(v);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int kwb_write(const char *kw, DESCR_t v) { return kwb_write_ent(kwb_find(kw), v); }   /* KW-3: same thin-wrapper split as the read side -- the protection verdict and the oracle-measured value-test-before-protected-test ORDER live in ONE body that both the by-name and the by-index writer execute. */
/*========================================================================================================================================================================================================*/
/* KW-3 (GOAL-SNOBOL4-100 D-3): THE STATIC KEYWORD SLOT, EMITTED IN THE PROGRAM'S ASM. The emitter resolves a `&KW` reference to its CANONICAL BLOCK INDEX at compile time (rt_kw_index), seals that index as a */
/* static quad in the emitted program (x86_ro_seal_q, both media), and the box loads it rip-relative and calls the by-index accessor below. That retires the measured KW-2 read cost -- a pointer to the */
/* keyword's NAME STRING plus a two-fold + ~60-arm strcmp cascade -- in favour of one rip-relative load and an O(1) array index. ⛔ THE INDEX IS A COMPILE-TIME/RUN-TIME CONTRACT: it is an offset into the */
/* block that g_kwb_bound names, so emit-time and run-time must see the SAME block. Today nothing calls rt_kw_bind(), so both see the file-scope g_kwb and the indices are stable by construction; the */
/* bounds test below is the tripwire that keeps a future rebind (KW-3b relocation) from silently reading a neighbour's cell instead of failing loudly. Miss (idx < 0) is NOT an error -- it means the block */
/* does not name that keyword (&ARB/&BAL/&REM/&FAIL and the whole pattern family), and the emitter then keeps the legacy name-string arm verbatim, which is what preserves their PATTERN values. */
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
}   /* kwb_init_once() is called HERE and not only in kwb_find because the by-index path deliberately bypasses the finder -- dropping it would leave the lazy seeding to whichever by-name read happened to run first, which is exactly the "initial value depends on access order" defect class KW-1 measured. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_kw_write_idx(int64_t idx, DESCR_t v) {
    if (idx < 0 || idx >= (int64_t)g_kwb_bound_n) return;
    kwb_init_once();
    (void)kwb_write_ent(&g_kwb_bound[idx], v);
}
/*========================================================================================================================================================================================================*/
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
            for (int c=1;c<128;c++) ascii_str[c-1]=(char)c; ascii_str[127]='\0';
            char *stable = rt_ws_strdup(cset_canonical(ascii_str));
            if (g_kw_cset_count < KW_CSET_MAX) {
                g_kw_cset_names[g_kw_cset_count].ptr  = stable;
                g_kw_cset_names[g_kw_cset_count].name = "&ascii";
                g_kw_cset_names[g_kw_cset_count].len  = 128;
                g_kw_cset_count++;
            }
            cs = stable;
        }
        return CSETVAL(cs);
    }
    if (!strcmp(kw,"cset")) {
        static const char *cs = NULL;
        if (!cs) {
            static char cset_str[256];
            for (int c=1;c<256;c++) cset_str[c-1]=(char)c; cset_str[255]='\0';
            char *stable = rt_ws_strdup(cset_canonical(cset_str));
            if (g_kw_cset_count < KW_CSET_MAX) {
                g_kw_cset_names[g_kw_cset_count].ptr  = stable;
                g_kw_cset_names[g_kw_cset_count].name = "&cset";
                g_kw_cset_names[g_kw_cset_count].len  = 256;
                g_kw_cset_count++;
            }
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
    { extern long g_trim, g_maxlngth;   /* ZW-4: g_anchor is now static in this file, no extern needed */
      if (!strcmp(kw,"anchor"))   return INTVAL(g_anchor);
      if (!strcmp(kw,"trim"))     return INTVAL(g_trim);
      if (!strcmp(kw,"maxlngth")) return INTVAL(g_maxlngth);
      if (!strcmp(kw,"fullscan")) return INTVAL(0);
      if (!strcmp(kw,"stlimit"))  return INTVAL(-1);
    }
    { extern long g_stno, g_stcount, g_lastno;
      if (!strcmp(kw,"stno"))     return INTVAL(g_stno);
      if (!strcmp(kw,"stcount"))  return INTVAL(g_stcount);
      if (!strcmp(kw,"lastno"))   return INTVAL(g_lastno);
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
/*--- snobol4 keyword reader: own entry over the shared kw table; uppercase-fold handles &ALPHABET etc. ---*/
DESCR_t rt_keyword_read_snobol4(const char *sval) {
    if (!sval) return NULVCL;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    char lk[64]; size_t li = 0;
    for (; kw[li] && li < sizeof(lk) - 1; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    if (rt_kw_static_on()) { DESCR_t bv; if (kwb_read(lk, &bv)) return bv; }   /* KW-2: the block answers first when armed. A MISS falls through untouched -- that is what keeps &ARB/&BAL/&REM/&FAIL working, since pattern keywords are deliberately NOT in the block and reach their PATTERN value via the bare-name variable table below (s145 oracle truth: bare ARB/BAL/REM/FAIL are PATTERN in BOTH engines). */
    if (!strcmp(lk, "alphabet")) { extern char alphabet[257]; return BSTRVAL(alphabet, 256); }
    if (!strcmp(lk, "digits")) return STRVAL("0123456789");
    if (!strcmp(lk, "ht")) return STRVAL("\t");
    if (!strcmp(lk, "lf") || !strcmp(lk, "nl")) return STRVAL("\n");
    if (!strcmp(lk, "vt")) return STRVAL("\x0B");
    if (!strcmp(lk, "ff")) return STRVAL("\x0C");
    if (!strcmp(lk, "cr")) return STRVAL("\r");
    if (!strcmp(lk, "esc")) return STRVAL("\x1B");
    if (!strcmp(lk, "errtext")) return g_sno_errtext ? STRVAL(g_sno_errtext) : STRVAL("");
    DESCR_t kv = kw_read(lk);
    if (!IS_FAIL(kv)) return kv;
    if (!strcmp(lk,"arb") || !strcmp(lk,"bal") || !strcmp(lk,"rem") || !strcmp(lk,"fail") || !strcmp(lk,"fence") || !strcmp(lk,"abort") || !strcmp(lk,"succeed")) { const char *bn = sval[0] == '&' ? sval + 1 : sval; return NV_GET_fn(bn); }   /* ⛔ THE PRIMITIVE-PATTERN KEYWORD FAMILY IS NOT A USER CONSTANT (manual Ch.16 p.187-188: "&ARB The primitive pattern ARB", and likewise &BAL/&REM/&FAIL/&FENCE/&ABORT/&SUCCEED -- protected read-only repositories of fundamental system patterns, present "only for historic reasons" because SPITBOL forbids altering ARB/BAL). They are deliberately absent from the KW-2 block and have ALWAYS reached their PATTERN value through the bare-name variable table; CN-2's canonical "&Name" re-key severed exactly that bridge, so every one of them fell into the tier-3 user-constant arm below and raised error 342 on READ. Measured: probe/kw/kw_datatypes m3+m4 PASS->DIFF between s146 (6/10) and this seat's baseline (4/10), oracle expecting ARB=PATTERN through SUCCEED=PATTERN. Restored ahead of tier-3 rather than inside it because a keyword the MANUAL names can never be an "unknown &name". */
    { char kb[128]; const char *ck = sval; if (sval[0] != '&') { kb[0] = '&'; size_t bl = strlen(sval); if (bl > 126) bl = 126; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }   /* SN4-CONSTANTS CN-2 (s145): tier-3 = unknown &name = USER CONSTANT, NV-keyed "&Name" -- the lexer strips '&', which aliased &W onto plain W (measured: amp=bare); canonicalizing here separates the namespaces */
      extern int NV_EXISTS_fn(const char *);
      if (!NV_EXISTS_fn(ck)) { char eb[192]; snprintf(eb, sizeof eb, "&constant read before its one-time assignment: %s", ck); core_runtime_error(342, eb); return NULVCL; }
      return NV_GET_fn(ck); }
}
/*--- per-statement entry hook: &STNO = current source stmt number, &LASTNO = previous, &STCOUNT = running executed tally (manual p195/p158) ---*/
void rt_stmt_enter(long stno) {
    g_lastno = g_stno;
    g_stno = stno;
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
void rt_keyword_write_snobol4(const char *sval, DESCR_t v) {
    if (!sval) return;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    char lk[64]; size_t li = 0;
    for (; kw[li] && li < sizeof(lk) - 1; li++) lk[li] = (kw[li] >= 'A' && kw[li] <= 'Z') ? (char)(kw[li] - 'A' + 'a') : kw[li];
    lk[li] = '\0';
    if (rt_kw_static_on() && kwb_write(lk, v)) return;   /* KW-2: the block owns every keyword it names, INCLUDING the protection verdict. A miss falls through to the legacy arms so pattern/unlisted keywords are unaffected. */
    long iv = 0;
    if (IS_INT(v)) iv = (long)v.i;
    else if (IS_REAL(v)) iv = (long)v.r;
    else { const char *s2 = VARVAL_fn(v); if (s2) iv = strtol(s2, (char **)0, 10); }
    if (!strcmp(lk,"anchor"))   { g_anchor = iv; if (g_rtcc_on) rtccb[RTCC_SLOT_R8] = (uint64_t)(int64_t)iv; return; }   /* RC-5: BLOCK-CANONICAL LAW — C write to claimed global must update block slot so reload sees new value */
    if (!strcmp(lk,"trim"))     { g_trim = iv; return; }
    if (!strcmp(lk,"maxlngth")) { g_maxlngth = iv; return; }
    if (!strcmp(lk,"error"))    { g_error = iv; return; }
    if (!strcmp(lk,"trace"))    { g_trace = iv; return; }
    if (!strcmp(lk,"dump"))     { g_dump = iv; return; }
    if (!strcmp(lk,"random"))   { g_random = iv; bb_rnd_seed = (unsigned long)iv; return; }
    if (!strcmp(lk,"fullscan") || !strcmp(lk,"stlimit") || !strcmp(lk,"abend") || !strcmp(lk,"code")) return;
    { char kb[128]; const char *ck = sval; if (sval[0] != '&') { kb[0] = '&'; size_t bl = strlen(sval); if (bl > 126) bl = 126; memcpy(kb + 1, sval, bl); kb[bl + 1] = 0; ck = kb; }   /* SN4-CONSTANTS CN-2: same canonical "&Name" key as the read side; NV_SET_fn's create marks is_const and its update path enforces the one-time seal (error 341) */
      NV_SET_fn(ck, v); }
}
