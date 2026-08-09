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
    return NV_GET_fn(sval);
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
    long iv = 0;
    if (IS_INT(v)) iv = (long)v.i;
    else if (IS_REAL(v)) iv = (long)v.r;
    else { const char *s2 = VARVAL_fn(v); if (s2) iv = strtol(s2, (char **)0, 10); }
    if (!strcmp(lk,"anchor"))   { g_anchor = iv; if (g_rtcc_on) g_rtcc_block[RTCC_SLOT_R8] = (uint64_t)(int64_t)iv; return; }   /* RC-5: BLOCK-CANONICAL LAW — C write to claimed global must update block slot so reload sees new value */
    if (!strcmp(lk,"trim"))     { g_trim = iv; return; }
    if (!strcmp(lk,"maxlngth")) { g_maxlngth = iv; return; }
    if (!strcmp(lk,"error"))    { g_error = iv; return; }
    if (!strcmp(lk,"trace"))    { g_trace = iv; return; }
    if (!strcmp(lk,"dump"))     { g_dump = iv; return; }
    if (!strcmp(lk,"random"))   { g_random = iv; bb_rnd_seed = (unsigned long)iv; return; }
    if (!strcmp(lk,"fullscan") || !strcmp(lk,"stlimit") || !strcmp(lk,"abend") || !strcmp(lk,"code")) return;
    NV_SET_fn(sval, v);
}
