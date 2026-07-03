#include "core.h"
#include "sil_macros.h"
#include "keywords.h"
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
int  g_jcon   = 0;
#define KW_CSET_MAX 16
static struct { const char *ptr; const char *name; int len; } g_kw_cset_names[KW_CSET_MAX];
static int g_kw_cset_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t make_kw_cset(const char *chars, const char *kw_name) {
    for (int i = 0; i < g_kw_cset_count; i++)
        if (!strcmp(g_kw_cset_names[i].name, kw_name))
            return CSETVAL(g_kw_cset_names[i].ptr);
    const char *arena = cset_canonical(chars);
    char *stable = GC_strdup(arena);
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
const char *kw_cset_name(const char *ptr) {
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr) return g_kw_cset_names[i].name;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kw_cset_len(const char *ptr) {
    for (int i = 0; i < g_kw_cset_count; i++)
        if (g_kw_cset_names[i].ptr == ptr) return g_kw_cset_names[i].len;
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
    if (!strcmp(kw,"ascii")) {
        static const char *cs = NULL;
        if (!cs) {
            static char ascii_str[128];
            for (int c=1;c<128;c++) ascii_str[c-1]=(char)c; ascii_str[127]='\0';
            const char *tmp = cset_canonical(ascii_str);
            int tlen = (int)strlen(tmp);
            char *stable = GC_malloc(tlen + 2);
            stable[0] = '\0'; memcpy(stable+1, tmp, tlen+1);
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
            const char *tmp = cset_canonical(cset_str);
            int tlen = (int)strlen(tmp);
            char *stable = GC_malloc(tlen + 2);
            stable[0] = '\0'; memcpy(stable+1, tmp, tlen+1);
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
    { extern long g_error, g_trace, g_dump, g_random;
      if (!strcmp(kw,"error"))  return INTVAL(g_error);
      if (!strcmp(kw,"trace"))  return INTVAL(g_trace);
      if (!strcmp(kw,"dump"))   return INTVAL(g_dump);
      if (!strcmp(kw,"random")) return INTVAL(g_random);
    }
    if (!strcmp(kw,"col"))     return INTVAL(0);
    if (!strcmp(kw,"row"))     return INTVAL(0);
    if (!strcmp(kw,"x"))       return INTVAL(0);
    if (!strcmp(kw,"y"))       return INTVAL(0);
    { extern int frame_depth; if (!strcmp(kw,"level")) return INTVAL(frame_depth); }
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
          char *buf = GC_malloc(16);
          snprintf(buf,16,"%04d/%02d/%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"dateline")) {
          char *buf = GC_malloc(64);
          strftime(buf,64,"%A, %B %e, %Y  %H:%M:%S",tm);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"clock")) {
          char *buf = GC_malloc(16);
          snprintf(buf,16,"%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
          return STRVAL(buf);
      }
      if (!strcmp(kw,"time")) {
          char *buf = GC_malloc(16);
          snprintf(buf,16,"%d",(int)(clock()*1000/CLOCKS_PER_SEC));
          return STRVAL(buf);
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
    DESCR_t kv = kw_read(kw);
    if (!IS_FAIL(kv)) return kv;
    return NV_GET_fn(sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_gen(const char *sval, long idx) {
    if (!sval) return FAILDESCR;
    const char *kw = sval[0] == '&' ? sval + 1 : sval;
    if (!strcmp(kw,"features")) {
        static const char *feats[] = { "UNIX", "ASCII", "co-expressions", "large integers", "string invocation", "native execution" };
        int n = (int)(sizeof(feats) / sizeof(feats[0]));
        if (idx < 0 || idx >= n) return FAILDESCR;
        return STRVAL(feats[idx]);
    }
    if (!strcmp(kw,"regions")) {
        if (idx == 0) return INTVAL(0);
        if (idx == 1 || idx == 2) return INTVAL((long)GC_get_heap_size());
        return FAILDESCR;
    }
    if (!strcmp(kw,"storage")) {
        long used = (long)(GC_get_heap_size() - GC_get_free_bytes());
        if (idx == 0) return INTVAL(0);
        if (idx == 1 || idx == 2) return INTVAL(used);
        return FAILDESCR;
    }
    if (!strcmp(kw,"collections")) {
        long g = (long)GC_get_gc_no();
        if (idx == 0) return INTVAL(g);
        if (idx == 1) return INTVAL(0);
        if (idx == 2) return INTVAL(g);
        if (idx == 3) return INTVAL(0);
        return FAILDESCR;
    }
    return FAILDESCR;
}
