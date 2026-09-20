#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <link.h>
#include <time.h>
#include <dlfcn.h>
#include "rt_slab.h"
#include "rt_arena.h"
#include "gc_heap.h"
#define GC_HEAP_MB 512
#include "descr.h"
#include "pin_va.h"
#include "gc_frame_map.h"
_Static_assert(sizeof(rt_hblk_t) == 16, "rt_hblk_t must be one 16-byte title unit");
typedef struct rt_hp_fr_t { char *top; char *end; long blocks; int armed; int _pad; char *virgin; int zfull; int _pad2; char *line; long alloc_total; long alloc_str; } rt_hp_fr_t;
rt_hp_fr_t g_hp_fr = { (char *)0, (char *)0, 0, 0, 0, (char *)0, -1, 0, (char *)0, 0, 0 };
_Static_assert(sizeof(rt_hp_fr_t) == 72, "RTX-2 extends the PL-SINK-3 cell; 0/8/16/24 stay put; the pacing line rides at 48; the two allocation counters ride at 56 and 64 so emitted code reaches them on the base it already holds");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, top)    ==  0, "PL-SINK-3 bakes g_hp_fr.top @0");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, end)    ==  8, "PL-SINK-3 bakes g_hp_fr.end @8");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, blocks) == 16, "PL-SINK-3 bakes g_hp_fr.blocks @16");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, armed)  == 24, "PL-SINK-3 bakes g_hp_fr.armed @24");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, virgin) == 32, "RTX-2 bakes g_hp_fr.virgin @32");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, zfull)  == 40, "RTX-2 bakes g_hp_fr.zfull @40");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, line)   == 48, "rtx_alloc.s compares the inline carve against g_hp_fr.line @48 (the pacing line, or the arena end when pacing is off)");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, alloc_total) == 56, "the &allocated total rides at 56: a hidden global cannot be named by emitted code and an exported one cannot be reached RIP-direct from rtx_alloc.s (measured: the shared object refuses to link)");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, alloc_str)   == 64, "the &allocated string total rides at 64");
#define g_hp_top    (g_hp_fr.top)
#define g_hp_end    (g_hp_fr.end)
#define g_hp_blocks (g_hp_fr.blocks)
#define g_hp_virgin (g_hp_fr.virgin)
static char *g_hp_arena = (char *)0;
static char *g_hp_gcline = (char *)0;
static char *g_hp_win = (char *)0;
static char *g_hp_wend = (char *)0;
static char *g_hp_cap_end = (char *)0;
static size_t g_hp_chunk = 0;
static long g_hp_grown = 0;
static int   g_hp_report_reg = 0;
static void gc_static_segs_init(void);
int g_gc_pending;
static long g_gc_polls = 0;
static int g_gc_in;
__attribute__((visibility("hidden"))) rt_sxt_fr_t g_sxt_fr = { (char *)0, 0, 0, -1 };
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, owner) ==  0, "rtx_str.s bakes g_sxt_fr.owner @0");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, len)   ==  8, "rtx_str.s bakes g_sxt_fr.len @8");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, gva_n) == 16, "rtx_str.s bakes g_sxt_fr.gva_n @16");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, off)   == 20, "rtx_str.s bakes g_sxt_fr.off @20");
#define g_sxt_len   (g_sxt_fr.len)
#define g_sxt_gva_n (g_sxt_fr.gva_n)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_line_mb(void)
{
    static long line_mb = -1;
    if (line_mb < 0) { const char *e = getenv("SCRIP_GC_LINE_MB"); line_mb = e ? atol(e) : 128; if (line_mb < 0) line_mb = 0; }
    return line_mb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_line_paced(void) { return gc_line_mb() > 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_line_span(long half)
{
    long line_mb = gc_line_mb();
    if (line_mb == 0) return half;
    { long span = line_mb << 20; return span < half ? span : half; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_gva_count(int n) { g_sxt_gva_n = n; }
void rt_sxt_break(const char *s) { rt_sxt_break_fast(s); }
void rt_sxt_frames_present(void) { g_sxt_fr.off = 1; g_sxt_owner = (char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_note(char *s, long len)
{
    g_sxt_owner = (char *)0;
    if (!s || !g_hp_arena || len < 0) return;
    if (s < g_hp_arena + sizeof(rt_hblk_t) || s >= g_hp_top) return;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if ((h->flags & HBF_TTL) && h->type == (uint16_t)DT_S && (char *)h + h->size == g_hp_top) { g_sxt_owner = s; g_sxt_len = len; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_sxt_match(const char *s)
{
    if (g_sxt_fr.off < 0) g_sxt_fr.off = 0;
    if (g_sxt_fr.off) return -1;
    return (s && s == g_sxt_owner) ? g_sxt_len : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_sxt_extend(char *s, long al, long bl)
{
    if (!s || s != g_sxt_owner || al != g_sxt_len || al < 0 || bl < 0) return (char *)0;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if (!(h->flags & HBF_TTL) || h->type != (uint16_t)DT_S || (char *)h + h->size != g_hp_top) { g_sxt_owner = (char *)0; return (char *)0; }
    DESCR_t *gv = (DESCR_t *)RT_GVA_VA;
    int refs = 0;
    { int k = 0, gn = g_sxt_gva_n;
      for (; k + 4 <= gn; k += 4) {
        if (__builtin_expect(gv[k].s == s, 0)     && gv[k].v == DT_S     && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 1].s == s, 0) && gv[k + 1].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 2].s == s, 0) && gv[k + 2].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        if (__builtin_expect(gv[k + 3].s == s, 0) && gv[k + 3].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; } }
      for (; k < gn; k++) if (gv[k].s == s && gv[k].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; } }
    uint64_t want = sizeof(rt_hblk_t) + (((uint64_t)(al + bl + 1) + 15u) & ~15ull);
    if (want > h->size) {
        uint64_t d = want - h->size;
        if (g_hp_top + d > g_hp_end) { g_sxt_owner = (char *)0; return (char *)0; }
        h->size = (uint32_t)want;
        g_hp_top += d; g_hp_fr.alloc_total += (long)d; g_hp_fr.alloc_str += (long)d;
    }
    { uint64_t pay = want - sizeof(rt_hblk_t); uint64_t used = (uint64_t)(al + bl + 1); if (pay > used) memset(s + used, 0, (size_t)(pay - used)); }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_verify(void)
{
    long n = 0;
    char *p = g_hp_arena;
    while (p && p < g_hp_top) {
        rt_hblk_t *h = (rt_hblk_t *)p;
        if (!(h->flags & HBF_TTL) || (h->size & 15u) || h->size < 2 * sizeof(rt_hblk_t) || p + h->size > g_hp_top) {
            fprintf(stderr, "[ZHP] heap verify FAILED at +%ld: flags=%x size=%u type=%u (corrupt title)\n", (long)(p - g_hp_arena), (unsigned)h->flags, (unsigned)h->size, (unsigned)h->type);
            abort();
        }
        n++; p += h->size;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_report(void)
{
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    long live = rt_gcheap_verify();
    fprintf(stderr, "[ZHP] arena=%dMB blocks=%ld(alloc'd)=%ld(walked) bytes=%ld verify=OK\n", (int)GC_HEAP_MB, g_hp_blocks, live, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_huge_advise(char *a0, char *e0)
{
    const char *nh = getenv("SCRIP_NOHUGE");
    if (nh && *nh && *nh != '0') return;
    { uintptr_t a = ((uintptr_t)a0 + 0x1FFFFFu) & ~(uintptr_t)0x1FFFFFu, e = ((uintptr_t)e0) & ~(uintptr_t)0x1FFFFFu; if (e > a) madvise((void *)a, (size_t)(e - a), MADV_HUGEPAGE); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_gcheap_grow(uint64_t need)
{
    size_t left = (size_t)(g_hp_cap_end - g_hp_end);
    if (!left) return 0;
    { size_t want = (size_t)need + 16u; if (want < (size_t)0x200000u) want = (size_t)0x200000u; want = (want + 0x1FFFFFu) & ~(size_t)0x1FFFFFu; if (want > left) want = left;
      if (mprotect(g_hp_end, want, PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZHP] soft end could not advance %ld MB at %p (reserve cap %ld MB)\n", (long)(want >> 20), (void *)g_hp_end, (long)((g_hp_cap_end - g_hp_arena) >> 20)); return 0; }
      gc_huge_advise(g_hp_end, g_hp_end + want);
      g_hp_end += want;
      g_hp_grown += (long)want;
      if (!gc_line_paced()) g_hp_fr.line = g_hp_end;
      if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZHP] soft end -> %ld MB committed of %ld MB reserved (grown %ld MB total)\n", (long)((g_hp_end - g_hp_arena) >> 20), (long)((g_hp_cap_end - g_hp_arena) >> 20), (long)(g_hp_grown >> 20));
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_init(void)
{
    long mb = (long)GC_HEAP_MB, cap_mb;
    { const char *e = getenv("SCRIP_HEAP_MB"); if (e && *e) { long v = atol(e); if (v >= 1 && v <= 4096) mb = v; } }
    cap_mb = (mb * 8 > (long)GC_HEAP_MB * 8) ? mb * 8 : (long)GC_HEAP_MB * 8;
    { const char *e = getenv("SCRIP_HEAP_MAX_MB"); if (e && *e) { long v = atol(e); if (v >= mb) cap_mb = v; } }
    { void *rv = mmap((void *)0, (size_t)cap_mb << 20, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
      if (rv == MAP_FAILED) { fprintf(stderr, "[ZHP] heap reserve mmap failed (%ld MB reserve) -- lower SCRIP_HEAP_MAX_MB\n", cap_mb); abort(); }
      g_hp_arena = (char *)rv; }
    g_hp_cap_end = g_hp_arena + ((size_t)cap_mb << 20);
    g_hp_chunk = (size_t)mb << 20;
    if (mprotect(g_hp_arena, g_hp_chunk, PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZHP] heap window commit failed (%ld MB of a %ld MB reserve)\n", mb, cap_mb); abort(); }
    g_hp_top = g_hp_arena; g_hp_end = g_hp_arena + g_hp_chunk;
    gc_huge_advise(g_hp_arena, g_hp_end);
    g_hp_virgin = g_hp_arena;
    g_hp_gcline = g_hp_arena + gc_line_span((long)(((size_t)mb << 20) >> 1));
    g_hp_fr.line = gc_line_paced() ? g_hp_gcline : g_hp_end;
    gc_static_segs_init();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_gcheap_carve(char *at, uint64_t total, uint16_t type)
{
    rt_hblk_t *h = (rt_hblk_t *)at;
    h->fwd = 0; h->size = (uint32_t)total; h->type = type; h->flags = HBF_TTL;
    g_hp_fr.alloc_total += (long)total; if (type == (uint16_t)DT_S) g_hp_fr.alloc_str += (long)total;
    uint64_t pay = total - sizeof(rt_hblk_t);
    if (g_hp_fr.zfull < 0) g_hp_fr.zfull = 0;
    { const int zfull = g_hp_fr.zfull;
    int fresh = !zfull && at >= g_hp_virgin;
    if (at + total > g_hp_virgin) g_hp_virgin = at + total;
    if (fresh) { }
    else if (!zfull && pay > 32 && (type == (uint16_t)DT_S || type == HB_WSC)) memset((char *)(h + 1) + (pay - 32), 0, 32);
    else memset((void *)(h + 1), 0, (size_t)pay);
    g_hp_blocks += 1;
    return (void *)(h + 1); }
}
static long g_ah_tn[512]; static long g_ah_tb[512]; static struct { void *ra; uint16_t type; long n; long b; } g_ah_ra[4096]; static int g_ah_reg = 0;
__attribute__((visibility("hidden"))) int g_ah_on = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_alloc_hist_report(void)
{
    fprintf(stderr, "[AH] per-type (type n bytes):\n");
    for (int t = 0; t < 512; t++) if (g_ah_tn[t]) fprintf(stderr, "[AH] T %d %ld %ld\n", t, g_ah_tn[t], g_ah_tb[t]);
    fprintf(stderr, "[AH] per-callsite (ra type n bytes):\n");
    for (int i = 0; i < 4096; i++) if (g_ah_ra[i].n) { Dl_info di; const char *sym = (dladdr(g_ah_ra[i].ra, &di) && di.dli_sname) ? di.dli_sname : "?"; long off = (dladdr(g_ah_ra[i].ra, &di) && di.dli_saddr) ? (long)((char *)g_ah_ra[i].ra - (char *)di.dli_saddr) : 0; fprintf(stderr, "[AH] R %p %d %ld %ld  %s+%ld\n", g_ah_ra[i].ra, (int)g_ah_ra[i].type, g_ah_ra[i].n, g_ah_ra[i].b, sym, off); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int rt_alloc_hist_on(void)
{
    if (g_ah_on < 0) { const char *e = getenv("SCRIP_ALLOC_HIST"); g_ah_on = (e && *e && *e != '0') ? 1 : 0; if (g_ah_on && !g_ah_reg) { g_ah_reg = 1; atexit(rt_alloc_hist_report); } }
    return g_ah_on;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_alloc_hist_init(void) { (void)rt_alloc_hist_on(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_alloc_hist_ra(void *ra, uint16_t type, uint64_t bytes)
{
    unsigned h = (unsigned)(((uintptr_t)ra >> 3) ^ (uintptr_t)type) & 4095u;
    for (unsigned k = 0; k < 4096; k++) { unsigned i = (h + k) & 4095u;
        if (!g_ah_ra[i].n) { g_ah_ra[i].ra = ra; g_ah_ra[i].type = type; g_ah_ra[i].n = 1; g_ah_ra[i].b = (long)bytes; return; }
        if (g_ah_ra[i].ra == ra && g_ah_ra[i].type == type) { g_ah_ra[i].n += 1; g_ah_ra[i].b += (long)bytes; return; } }
}
static long gc_plant_pin_skip(void)
{
    static long n = -2;
    if (n == -2) { const char *e = getenv("SCRIP_GC_PLANT_PIN_SKIP"); n = (e && *e) ? atol(e) : -1; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_plant_pin_type(void)
{
    static long t = -2;
    if (t == -2) { const char *e = getenv("SCRIP_GC_PLANT_PIN_TYPE"); t = (e && *e) ? atol(e) : -1; }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_alloc_detax = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes)
{
    if (g_alloc_detax == 1 && g_ah_on <= 0) { uint64_t tf = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull); if (gc_line_paced() && g_hp_gcline && !g_gc_in && g_hp_top + tf > g_hp_gcline) g_gc_pending = 1; if (g_hp_top + tf <= g_hp_end) { void *rf = rt_gcheap_carve(g_hp_top, tf, type); g_hp_top += tf; return rf; } }
    if (g_ah_on > 0) { unsigned t = (unsigned)type & 511u; g_ah_tn[t] += 1; g_ah_tb[t] += (long)payload_bytes; }
    uint64_t total = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
    void *r;
    static long stress_n = -1, stress_c = 0;
    if (!g_hp_report_reg) { g_hp_report_reg = 1; atexit(rt_gcheap_report); }
    if (!g_hp_arena) rt_gcheap_init();
    if (stress_n < 0) { const char *e = getenv("SCRIP_GC_STRESS"); stress_n = e ? atol(e) : 0; }
    if (stress_n > 0 && ++stress_c >= stress_n) { stress_c = 0; g_gc_pending = 1; }
    { static long since = 0, budget = -1;
      if (budget < 0) { const char *e = getenv("SCRIP_GC_BUDGET_MB"); long mb = e ? atol(e) : 0; budget = mb > 0 ? (mb << 20) : 0; }
      if (!g_alloc_detax) g_alloc_detax = (stress_n == 0 && budget == 0 && g_ah_on <= 0 && g_hp_arena && g_hp_report_reg) ? 1 : -1;
      g_hp_fr.armed = (g_alloc_detax == 1 && g_ah_on <= 0) ? 1 : 0;
      if (budget) { since += (long)total; if (since >= budget && (g_hp_top - g_hp_arena) * 2 >= (g_hp_end - g_hp_arena)) { since = 0; g_gc_pending = 2; } } }
    if (gc_line_paced() && g_hp_gcline && !g_gc_in && g_hp_top + total > g_hp_gcline && g_hp_top + total <= g_hp_end) g_gc_pending = 1;
    if (g_hp_top + total > g_hp_end && g_hp_win + total > g_hp_wend) { g_gc_pending = 1; rt_gcheap_grow(total); }
    if (g_hp_top + total <= g_hp_end) { r = rt_gcheap_carve(g_hp_top, total, type); g_hp_top += total; return r; }
    if (g_hp_win + total <= g_hp_wend) {
        uint64_t avail = (uint64_t)(g_hp_wend - g_hp_win);
        if (avail - total == sizeof(rt_hblk_t)) total += sizeof(rt_hblk_t);
        r = rt_gcheap_carve(g_hp_win, total, type);
        g_hp_win += total;
        if (g_hp_win < g_hp_wend) { rt_hblk_t *fl = (rt_hblk_t *)g_hp_win; fl->fwd = 0; fl->size = (uint32_t)(g_hp_wend - g_hp_win); fl->type = HB_FILL; fl->flags = HBF_TTL; }
        return r;
    }
    fprintf(stderr, "[ZHP] heap exhausted at the reserve cap (%ld MB committed of %ld MB reserved, %ld blocks live) -- THIS REQUEST wanted %llu payload bytes (%llu with the header) of block kind %u. A request larger than the whole reserve is a CORRUPTED LENGTH, not a big program: a length computed from a pointer a collection moved asks for the world, so read the size before raising SCRIP_HEAP_MAX_MB -- which is IGNORED below the committed window anyway (rt_gcheap_init takes the env cap only when it is >= the window). If the size is sane, then a safe point is missing and nothing ever collected.\n", (long)((g_hp_end - g_hp_arena) >> 20), (long)((g_hp_cap_end - g_hp_arena) >> 20), g_hp_blocks, (unsigned long long)payload_bytes, (unsigned long long)total, (unsigned)type);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *c_rt_str_alloc(long n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)DT_S, 0);
    long want = (n < 0 ? 0 : n) + 1;
    _Static_assert(DT_S < HB_ZCOL, "value-world heap types carry DTYPE_t verbatim: DT_S is the ONLY DTYPE_t ever passed as a block type (every other caller passes HB_*), so the invariant is that it can never be mistaken for one -- it is NOT that DT_S holds any particular value. This assert read DT_S == 1 until s230, which pinned an incidental number instead of the property its own message names, and therefore fired on the TAG-3 class-bit renumber while the property still held.");
    return (char *)rt_gcheap_alloc((uint16_t)DT_S, (uint64_t)want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_str_dup(const char *s)
{
    if (!s) s = "";
    long n = (long)strlen(s);
    char *b = rt_str_alloc(n);
    if (b) memcpy(b, s, (size_t)n);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_wsb_alloc(size_t n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSB, (uint64_t)n);
    return rt_gcheap_alloc((uint16_t)HB_WSB, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_ws_alloc_descr(size_t n)
{
    size_t b = (n ? n : 1) * sizeof(DESCR_t);
    void *q;
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_DVEC, (uint64_t)b);
    q = rt_gcheap_alloc((uint16_t)HB_DVEC, (uint64_t)b);
    if (q) memset(q, 0, b);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_heap_alloc_c(size_t n)
{
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSC, 0);
    return rt_gcheap_alloc((uint16_t)HB_WSC, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_struct_alloc(uint16_t type, size_t n)
{
    if (type < HB_PLDB || type > HB_PLDBK) abort();
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), type, (uint64_t)n);
    return rt_gcheap_alloc(type, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pvec_alloc(size_t n)
{
    size_t b = (n ? n : 1) * sizeof(void *);
    void *q;
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_PVEC, (uint64_t)b);
    q = rt_gcheap_alloc((uint16_t)HB_PVEC, (uint64_t)b);
    if (q) memset(q, 0, b);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pvec_realloc(void *p, size_t n)
{
    if (!p) return rt_pvec_alloc(n);
    { rt_hblk_t *h = (rt_hblk_t *)p - 1; size_t old = (size_t)h->size - sizeof(rt_hblk_t), want = (n ? n : 1) * sizeof(void *);
      if (want <= old) return p;
      { void *q = rt_pvec_alloc(n); memcpy(q, p, old); return q; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pm_struct_alloc(uint16_t type, size_t n)
{
    if (type < HB_DTP || type > HB_DTPRCP) abort();
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), type, (uint64_t)n);
    return rt_gcheap_alloc(type, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_wsb_realloc(void *p, size_t n)
{
    if (!p) return rt_wsb_alloc(n);
    { rt_hblk_t *h = (rt_hblk_t *)p - 1; size_t old = (size_t)h->size - sizeof(rt_hblk_t);
      if (n <= old) return p;
      { void *q = rt_wsb_alloc(n); memcpy(q, p, old); return q; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_agg_alloc(int kind, size_t n)
{
    uint16_t ty = (uint16_t)(HB_AGGV + (kind < 0 ? 0 : (kind > 2 ? 2 : kind)));
    if (g_alloc_detax != 1 && rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), ty, (uint64_t)n);
    return rt_gcheap_alloc(ty, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_heap_strdup_c(const char *s)
{
    if (!s) return (char *)0;
    { size_t n = strlen(s); char *q = (char *)rt_heap_alloc_c(n + 1); memcpy(q, s, n + 1); return q; }
}
#include <setjmp.h>
#include "../core/core.h"
#include "rt_coexpr.h"
long g_scrip_coexpr_live;
static struct gc_rng_t { const char *lo, *hi; } *g_gc_rrng = (struct gc_rng_t *)0;
static long g_gc_rrng_n = 0, g_gc_rrng_cap = 0;
static struct gc_seg_t { char *lo, *hi; } *g_gc_segs = (struct gc_seg_t *)0;
static long g_gc_nseg = -1, g_gc_seg_cap = 0;
static rt_hblk_t **g_gc_idx = (rt_hblk_t **)0;
static rt_hblk_t **g_gc_idxbuf = (rt_hblk_t **)0;
static long g_gc_icap = 0;
static rt_hblk_t *g_gc_mhead = (rt_hblk_t *)0;
static rt_hblk_t **g_gc_liveo = (rt_hblk_t **)0;
static uint64_t *g_gc_livef = (uint64_t *)0;
static long g_gc_lcap = 0;
static long g_gc_nblk = 0;
static uint32_t *g_gc_pmap = (uint32_t *)0;
static void **g_gc_hs = (void **)0;
static long g_gc_hcap = 0, g_gc_hn = 0;
typedef struct gc_slot_t { rt_hblk_t *hloc; uintptr_t off; } gc_slot_t;
static gc_slot_t *g_gc_slots = (gc_slot_t *)0;
static long g_gc_nslot = 0, g_gc_scap = 0;
static int g_gc_in = 0;
static long g_gc_runs = 0, g_gc_interior = 0;
static char *g_gc_stktop = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GCBK_MAGIC 0x5a47424b48445200ull
typedef struct gcbk_head_t { uint64_t magic; uint64_t len; } gcbk_head_t;
static size_t gcbk_page(void) { static size_t pg = 0; if (!pg) pg = (size_t)sysconf(_SC_PAGESIZE); return pg; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *gcbk_alloc(size_t n)
{
    size_t pg = gcbk_page(), need = (n + sizeof(gcbk_head_t) + pg - 1) & ~(pg - 1);
    gcbk_head_t *h = (gcbk_head_t *)mmap((void *)0, need, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (h == MAP_FAILED) { fprintf(stderr, "[ZHP] collector bookkeeping mmap failed (%zu bytes)\n", need); abort(); }
    h->magic = GCBK_MAGIC; h->len = (uint64_t)need;
    return (void *)((uint8_t *)h + sizeof(gcbk_head_t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *gcbk_grow(void *p, size_t n)
{
    gcbk_head_t *h; size_t pg, need; void *nv;
    if (!p) return gcbk_alloc(n);
    h = (gcbk_head_t *)((uint8_t *)p - sizeof(gcbk_head_t));
    if (h->magic != GCBK_MAGIC) { fprintf(stderr, "[ZHP] gcbk_grow on a block the collector bookkeeping allocator did not hand out (%p)\n", p); abort(); }
    pg = gcbk_page(); need = (n + sizeof(gcbk_head_t) + pg - 1) & ~(pg - 1);
    if (need <= (size_t)h->len) return p;
    nv = mremap((void *)h, (size_t)h->len, need, MREMAP_MAYMOVE);
    if (nv == MAP_FAILED) { fprintf(stderr, "[ZHP] collector bookkeeping mremap failed (%zu to %zu bytes)\n", (size_t)h->len, need); abort(); }
    h = (gcbk_head_t *)nv; h->len = (uint64_t)need;
    return (void *)((uint8_t *)h + sizeof(gcbk_head_t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gcbk_drop(void *p)
{
    gcbk_head_t *h;
    if (!p) return;
    h = (gcbk_head_t *)((uint8_t *)p - sizeof(gcbk_head_t));
    if (h->magic != GCBK_MAGIC) { fprintf(stderr, "[ZHP] gcbk_drop on a block the collector bookkeeping allocator did not hand out (%p)\n", p); abort(); }
    h->magic = 0;
    munmap((void *)h, (size_t)h->len);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_live_grow(long need) { if (need < g_gc_lcap) return; g_gc_lcap = g_gc_lcap ? g_gc_lcap : 4096; while (g_gc_lcap <= need) g_gc_lcap *= 2;
    g_gc_liveo = (rt_hblk_t **)gcbk_grow((void *)g_gc_liveo, (size_t)g_gc_lcap * sizeof(*g_gc_liveo)); g_gc_livef = (uint64_t *)gcbk_grow((void *)g_gc_livef, (size_t)g_gc_lcap * sizeof(*g_gc_livef)); if (!g_gc_liveo || !g_gc_livef) abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double gc_walk_ns(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (double)t.tv_sec * 1e9 + (double)t.tv_nsec; }
static long gc_collect_ex(void);
static char *g_gc_seam_sp = (char *)0;
static DESCR_t *g_gc_shield_arr = (DESCR_t *)0;
static int g_gc_shield_n = 0;
static const char **g_gc_shield_r = (const char **)0;
static int g_gc_shield_r_is_probe = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_point_arr_body(DESCR_t *arr, int n, const char **r0, char *floor, int is_probe)
{
    int pv = g_gc_pending;
    if (!pv && g_hp_gcline && g_hp_top > g_hp_gcline) pv = 1;
    if (!pv) return;
    g_gc_pending = 0;
    g_gc_shield_arr = arr; g_gc_shield_n = n; g_gc_shield_r = r0; g_gc_shield_r_is_probe = is_probe;
    g_gc_seam_sp = floor;
    gc_collect_ex();
    g_gc_seam_sp = (char *)0;
    g_gc_shield_arr = (DESCR_t *)0; g_gc_shield_n = 0; g_gc_shield_r = (const char **)0; g_gc_shield_r_is_probe = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point_arr_c(DESCR_t *arr, int n, const char **r0, char *floor)
{
    gc_point_arr_body(arr, n, r0, floor, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point_arr_probe_c(DESCR_t *arr, int n, const char **saved_subject_reg, char *floor)
{
    gc_point_arr_body(arr, n, saved_subject_reg, floor, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point(DESCR_t *d0, const char **r0)
{
    rt_gc_point_arr(d0, d0 ? 1 : 0, r0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_hins(void *p)
{
    if (g_gc_hn * 10 >= g_gc_hcap * 7) {
        long ncap = g_gc_hcap ? g_gc_hcap * 2 : 4096; void **nh = (void **)gcbk_alloc((size_t)ncap * sizeof(void *));
        if (!nh) { fprintf(stderr, "[ZGC] visited-set alloc failed\n"); abort(); }
        for (long i = 0; i < g_gc_hcap; i++) if (g_gc_hs[i]) { uint64_t h = ((uint64_t)g_gc_hs[i] >> 4) * 0x9E3779B97F4A7C15ull; long s = (long)(h & (uint64_t)(ncap - 1)); while (nh[s]) s = (s + 1) & (ncap - 1); nh[s] = g_gc_hs[i]; }
        gcbk_drop(g_gc_hs); g_gc_hs = nh; g_gc_hcap = ncap;
    }
    { uint64_t h = ((uint64_t)p >> 4) * 0x9E3779B97F4A7C15ull; long s = (long)(h & (uint64_t)(g_gc_hcap - 1));
      while (g_gc_hs[s]) { if (g_gc_hs[s] == p) return 0; s = (s + 1) & (g_gc_hcap - 1); }
      g_gc_hs[s] = p; g_gc_hn++; return 1; }
}
static char *g_gc_pmap_top = (char *)0;
static long g_gc_dvec_elems = 0;
static long g_gc_dvec_nondvec = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_hblk_t *gc_blk_of(const char *p)
{
    if (!p || p < g_hp_arena || p >= g_hp_top || !g_gc_idx) return (rt_hblk_t *)0;
    if (g_gc_pmap && p < g_gc_pmap_top) { long i = (long)g_gc_pmap[(size_t)(p - g_hp_arena) >> 9]; if (i < g_gc_nblk) { rt_hblk_t *h = g_gc_idx[i]; while (i + 1 < g_gc_nblk && (char *)h + h->size <= p) h = g_gc_idx[++i]; if ((char *)h <= p && p < (char *)h + h->size) return h; } }
    { long lo = 0, hi = g_gc_nblk - 1; while (lo <= hi) { long m = (lo + hi) >> 1; char *b = (char *)g_gc_idx[m]; if (p < b) hi = m - 1; else if (p >= b + g_gc_idx[m]->size) lo = m + 1; else return g_gc_idx[m]; } return (rt_hblk_t *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_type_moves(uint16_t t) { return (t == HB_DVEC || (t >= HB_PLDB && t <= HB_DTPRCP) || t == HB_ARR || t == HB_DINST || HB_IS_AGG(t)) ? 1 : 0; }
static void gc_mark_blk(rt_hblk_t *h, uint16_t addf)
{
    uint16_t old = h->flags;
    h->flags = (uint16_t)(old | HBF_MARK | addf);
    if (!(old & HBF_MARK) && gc_type_moves(h->type)) { h->fwd = (uint64_t)(uintptr_t)g_gc_mhead; g_gc_mhead = h; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_visit_segment(const char *lo0, const char *hi0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_slot_reg(void *loc)
{
    const char *v = *(const char *const *)loc;
    if (!gc_blk_of(v)) return;
    if (!gc_hins((void *)((uintptr_t)loc | 1))) return;
    if (g_gc_nslot == g_gc_scap) { g_gc_scap = g_gc_scap ? g_gc_scap * 2 : 4096; g_gc_slots = (gc_slot_t *)gcbk_grow((void *)g_gc_slots, (size_t)g_gc_scap * sizeof(*g_gc_slots)); if (!g_gc_slots) abort(); }
    { rt_hblk_t *hl = gc_blk_of((const char *)loc);
      if (hl) { g_gc_slots[g_gc_nslot].hloc = hl; g_gc_slots[g_gc_nslot].off = (uintptr_t)((char *)loc - (char *)(hl + 1)); }
      else { g_gc_slots[g_gc_nslot].hloc = (rt_hblk_t *)0; g_gc_slots[g_gc_nslot].off = (uintptr_t)loc; }
      g_gc_nslot++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_raw(const char **loc)
{
    rt_hblk_t *h = gc_blk_of(*loc);
    if (!h) return;
    gc_mark_blk(h, 0);
    gc_slot_reg((void *)loc);
}
int rt_gc_ptr_in_heap_slot(const char *p) { return gc_blk_of(p) != (rt_hblk_t *)0; }
int rt_gc_slot_registered(const void *loc)
{
    void *k = (void *)((uintptr_t)loc | 1);
    if (!g_gc_hs || !g_gc_hcap) return 0;
    { uint64_t h = ((uint64_t)k >> 4) * 0x9E3779B97F4A7C15ull; long sl = (long)(h & (uint64_t)(g_gc_hcap - 1));
      while (g_gc_hs[sl]) { if (g_gc_hs[sl] == k) return 1; sl = (sl + 1) & (g_gc_hcap - 1); }
      return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_mark_agg(const void *p) { rt_hblk_t *h = gc_blk_of((const char *)p); if (h) gc_mark_blk(h, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_tbblk(struct _TBBLK_t *t);
static int gc_block_exact(const char *q, uint16_t want_type);
static void gc_wl_push(DESCR_t *d);
static void gc_visit_arblk(ARBLK_t *a);
static void gc_visit_datinst(DATINST_t *u);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_arblk(ARBLK_t *a)
{
    if (a->proto) rt_gc_visit_raw((const char **)&a->proto);
    if (!a->data) return;
    rt_gc_visit_raw((const char **)&a->data);
    { long n = (long)(a->hi - a->lo + 1); if (a->ndim == 2) n *= (long)(a->hi2 - a->lo2 + 1); if (n < 0) n = 0; for (long i = 0; i < n; i++) gc_wl_push(&a->data[i]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_datinst(DATINST_t *u)
{
    if (!u->fields || !u->type) return;
    rt_gc_visit_raw((const char **)&u->type);
    rt_gc_visit_raw((const char **)&u->fields);
    for (int i = 0; i < u->type->nfields; i++) gc_wl_push(&u->fields[i]);
}
static void gc_visit_vcell(VCELL_t *vc)
{
    gc_mark_agg(vc);
    if (vc->key) gc_mark_agg(vc->key);
    gc_slot_reg((void *)&vc->key);
    gc_slot_reg((void *)&vc->tbl);
    gc_slot_reg((void *)&vc->cellp);
    if (vc->cellp) gc_mark_agg((const void *)vc->cellp);
    if (vc->tbl && gc_hins((void *)vc->tbl)) gc_visit_tbblk(vc->tbl);
    rt_gc_visit_descr(&vc->key_d); rt_gc_visit_descr(&vc->sv);
    if (vc->cellp) rt_gc_visit_descr(vc->cellp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_tbblk(struct _TBBLK_t *t)
{
    gc_mark_agg(t);
    rt_gc_visit_descr(&t->dflt);
    if (t->ord) { rt_gc_visit_raw((const char **)&t->ord); for (unsigned i = 0; i < t->ord_len; i++) rt_gc_visit_descr(&t->ord[i]); }
    if (!t->buckets) return;
    rt_gc_visit_raw((const char **)&t->buckets);
    for (unsigned b = 0; b < t->nbuck; b++) {
        TBBUCK_t *bk = t->buckets[b];
        if (!bk) continue;
        rt_gc_visit_raw((const char **)&t->buckets[b]);
        for (unsigned i = 0; i < bk->len; i++) {
            TBPAIR_t *e = &bk->ent[i];
            if (e->key) rt_gc_visit_raw((const char **)&e->key);
            rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t **g_gc_wl = (DESCR_t **)0;
static long g_gc_wln = 0, g_gc_wlcap = 0, g_gc_wlmax = 0;
static int g_gc_wl_draining = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_wl_push(DESCR_t *d)
{
    if (!d) return;
    if (g_gc_wln == g_gc_wlcap) { g_gc_wlcap = g_gc_wlcap ? g_gc_wlcap * 2 : 4096; g_gc_wl = (DESCR_t **)gcbk_grow((void *)g_gc_wl, (size_t)g_gc_wlcap * sizeof(*g_gc_wl)); if (!g_gc_wl) { fprintf(stderr, "[ZGC] mark worklist alloc failed at %ld entries\n", g_gc_wlcap); abort(); } }
    g_gc_wl[g_gc_wln++] = d;
    if (g_gc_wln > g_gc_wlmax) g_gc_wlmax = g_gc_wln;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_one(DESCR_t *d)
{
    if (!d) return;
    switch ((int)d->v) {
    case DT_S: case DT_SNUL: case DT_X: {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (!h) return;
        gc_mark_blk(h, 0);
        if (d->s != (char *)(h + 1)) g_gc_interior++;
        gc_slot_reg((void *)&d->s);
        return; }
    case DT_A: {
        ARBLK_t *a = d->arr;
        if (!a || !gc_block_exact((const char *)a, HB_ARR)) return;
        gc_mark_agg((const void *)a);
        gc_slot_reg((void *)&d->arr);
        if (!gc_hins((void *)a)) return;
        gc_visit_arblk(a);
        return; }
    case DT_T: {
        TBBLK_t *t = d->tbl;
        if (!t || !gc_block_exact((const char *)t, HB_AGGT)) return;
        gc_slot_reg((void *)&d->tbl);
        if (!gc_hins((void *)t)) return;
        gc_visit_tbblk(t);
        return; }
    case DT_DATA: {
        if (d->slen == DATA_ELEMS_SLEN) { rt_hblk_t *eh = gc_blk_of((const char *)d->ptr);
            if (eh) { g_gc_dvec_elems++; if (eh->type != HB_DVEC) { g_gc_dvec_nondvec++; } }
            rt_gc_visit_raw((const char **)&d->ptr); return; }
        { DATINST_t *u = d->u;
          if (!u || !gc_block_exact((const char *)u, HB_DINST)) return;
          gc_mark_agg((const void *)u);
          gc_slot_reg((void *)&d->u);
          if (!gc_hins((void *)u)) return;
          gc_visit_datinst(u); }
        return; }
    case DT_N: {
        if (d->slen == 2) { VCELL_t *vc = (VCELL_t *)d->p; if (!vc || !gc_block_exact((const char *)vc, HB_AGGV)) return; gc_slot_reg((void *)&d->p); if (!gc_hins((void *)vc)) return; gc_visit_vcell(vc); return; }
        if (d->slen == 1) { DESCR_t *tc = (DESCR_t *)d->ptr; rt_hblk_t *th = gc_blk_of((const char *)tc); if (!th || (const char *)tc < (const char *)(th + 1) || (const char *)tc + 16 > (const char *)th + th->size) return; gc_slot_reg((void *)&d->ptr); gc_mark_agg((const void *)tc); if (gc_hins((void *)tc)) gc_wl_push(tc); return; }
        { rt_hblk_t *h = gc_blk_of(d->s); if (h) { gc_mark_blk(h, 0); gc_slot_reg((void *)&d->s); } }
        return; }
    case DT_P: {
        rt_hblk_t *h = gc_blk_of((const char *)d->p);
        if (!h) return;
        gc_mark_blk(h, 0);
        gc_slot_reg((void *)&d->p);
        return; }
    case DT_PLVAR: case DT_PLREF: {
        rt_hblk_t *h = gc_blk_of((const char *)d->p);
        if (!h) return;
        gc_mark_blk(h, 0);
        if ((const char *)d->p != (const char *)(h + 1)) g_gc_interior++;
        gc_slot_reg((void *)&d->p);
        return; }
    case DT_BIG: {
        rt_hblk_t *h = gc_blk_of((const char *)d->p);
        if (!h) return;
        gc_mark_blk(h, 0);
        gc_slot_reg((void *)&d->p);
        return; }
    default: return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_descr(DESCR_t *d)
{
    if (!d) return;
    if (g_gc_wl_draining) { gc_wl_push(d); return; }
    g_gc_wl_draining = 1;
    gc_wl_push(d);
    while (g_gc_wln > 0) gc_visit_one(g_gc_wl[--g_gc_wln]);
    g_gc_wl_draining = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long g_gc_rrng_ss = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add(const char *lo, const char *hi)
{
    if (g_gc_rrng_n == g_gc_rrng_cap) { g_gc_rrng_cap = g_gc_rrng_cap ? g_gc_rrng_cap * 2 : 64;
        g_gc_rrng = (struct gc_rng_t *)gcbk_grow((void *)g_gc_rrng, (size_t)g_gc_rrng_cap * sizeof(*g_gc_rrng)); if (!g_gc_rrng) abort(); }
    g_gc_rrng[g_gc_rrng_n].lo = lo; g_gc_rrng[g_gc_rrng_n].hi = hi; g_gc_rrng_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const gc_frame_map_t **g_gc_maps = (const gc_frame_map_t **)0;
static int g_gc_maps_n = 0, g_gc_maps_cap = 0, g_gc_map_report_reg = 0;
static long g_gc_map_checked = 0;
static void gc_frame_maps_dump_atexit(void);
static int gc_frame_map_registered(const gc_frame_map_t *m) { for (int i = 0; i < g_gc_maps_n; i++) if (g_gc_maps[i] == m) return 1; return 0; }
void rt_gc_frame_maps_add(const gc_frame_map_t *m)
{
    { static int dreg = 0; if (!dreg) { const char *e = getenv("SCRIP_GC_MAPS_DUMP"); dreg = 1; if (e && *e == '1') atexit(gc_frame_maps_dump_atexit); } }
    if (!m || gc_frame_map_registered(m)) return;
    if (m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-MAP] rt_gc_frame_maps_add: %p is not a frame map (magic %08x)\n", (const void *)m, m->magic); abort(); }
    if (g_gc_maps_n == g_gc_maps_cap) { g_gc_maps_cap = g_gc_maps_cap ? g_gc_maps_cap * 2 : 64;
        g_gc_maps = (const gc_frame_map_t **)gcbk_grow((void *)g_gc_maps, (size_t)g_gc_maps_cap * sizeof(*g_gc_maps)); if (!g_gc_maps) abort(); }
    g_gc_maps[g_gc_maps_n++] = m;
}
void rt_gc_frame_maps_install(const gc_frame_map_t *const *maps, int n) { for (int i = 0; i < n; i++) rt_gc_frame_maps_add(maps[i]); }
void rt_gc_frame_maps_install_counted(const void *tab) { const uint64_t *t = (const uint64_t *)tab; if (!t) return; rt_gc_frame_maps_install((const gc_frame_map_t *const *)(t + 1), (int)t[0]); }
const gc_frame_map_t *const *rt_gc_frame_maps(int *n) { if (n) *n = g_gc_maps_n; return g_gc_maps; }
void rt_gc_frame_maps_dump(void)
{
    int n = 0; const gc_frame_map_t *const *t = rt_gc_frame_maps(&n);
    fprintf(stderr, "[GC-MAPTAB] n=%d\n", n);
    for (int i = 0; i < n; i++) {
        const gc_frame_map_t *m = t[i];
        if (!m || m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-MAPTAB] BAD entry %d %p\n", i, (const void *)m); continue; }
        fprintf(stderr, "[GC-MAPTAB] graph=%s frame_bytes=%u header_bytes=%u flags=%u map_off=%u\n", m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, m->flags, (unsigned)m->map_off);
        if (m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT)) { const uint64_t *t = (const uint64_t *)(m + 1); long k = (long)t[0]; fprintf(stderr, "[GC-MAPTAB-LAYOUT] graph=%s n=%ld", m->graph_name ? m->graph_name : "?", k); for (long j = 0; j < k; j++) fprintf(stderr, " %d:%u:%d", GC_LAY_OFF(t[1 + j]), GC_LAY_KIND(t[1 + j]), GC_LAY_SIZE(t[1 + j])); fprintf(stderr, "\n"); }
    }
}
static void gc_frame_maps_dump_atexit(void) { rt_gc_frame_maps_dump(); }
static void gc_frame_map_report(void) { fprintf(stderr, "[GC-MAP] frames_checked=%ld maps=%d\n", g_gc_map_checked, g_gc_maps_n); }
void rt_gc_frame_map_check(const DESCR_t *cell)
{
    const gc_frame_map_t *m = cell ? (const gc_frame_map_t *)cell->p : (const gc_frame_map_t *)0;
    int reg = gc_frame_map_registered(m);
    if (!cell || cell->v != DT_MAP || !m || m->magic != GC_FRAME_MAP_MAGIC || m->frame_bytes != cell->slen || !reg) {
        fprintf(stderr, "[GC-MAP] BAD CELL at %p: v=0x%02x slen=%u map=%p magic=%08x frame_bytes=%u registered=%d graph=%s\n", (const void *)cell, cell ? cell->v : 0, cell ? cell->slen : 0u, (const void *)m, (m && reg) ? m->magic : 0u, (m && reg) ? m->frame_bytes : 0u, reg, (m && reg && m->graph_name) ? m->graph_name : "?");
        abort();
    }
    if (!g_gc_map_report_reg) { g_gc_map_report_reg = 1; atexit(gc_frame_map_report); }
    g_gc_map_checked++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add_topword(const char *lo)
{
    rt_gc_root_range_add(lo, (const char *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add_seamsafe(const char *lo, const char *hi)
{
    rt_gc_root_range_add(lo, hi); g_gc_rrng_ss++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_del(const char *lo)
{
    for (long i = 0; i < g_gc_rrng_n; i++) if (g_gc_rrng[i].lo == lo) { g_gc_rrng[i] = g_gc_rrng[--g_gc_rrng_n]; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_phdr_cb(struct dl_phdr_info *info, size_t sz, void *data)
{
    const char *nm = info->dlpi_name;
    (void)sz; (void)data;
    if (nm && nm[0] && !strstr(nm, "libscrip")) return 0;
    for (int i = 0; i < (int)info->dlpi_phnum; i++) { const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD || !(ph->p_flags & PF_W)) continue;
        if (g_gc_nseg == g_gc_seg_cap) { g_gc_seg_cap = g_gc_seg_cap ? g_gc_seg_cap * 2 : 16;
            g_gc_segs = (struct gc_seg_t *)gcbk_grow((void *)g_gc_segs, (size_t)g_gc_seg_cap * sizeof(*g_gc_segs)); if (!g_gc_segs) abort(); }
        g_gc_segs[g_gc_nseg].lo = (char *)(info->dlpi_addr + ph->p_vaddr); g_gc_segs[g_gc_nseg].hi = g_gc_segs[g_gc_nseg].lo + ph->p_memsz; g_gc_nseg++; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_static_segs_init(void)
{
    if (g_gc_nseg >= 0) return;
    g_gc_nseg = 0;
    dl_iterate_phdr(gc_phdr_cb, (void *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gcheap_warmup(void)
{
    gc_static_segs_init();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_stack_region(char **lo, char **hi)
{
    FILE *f = fopen("/proc/self/maps", "r"); char ln[256]; unsigned long a = 0, b = 0;
    if (f) { while (fgets(ln, sizeof ln, f)) if (strstr(ln, "[stack]")) { sscanf(ln, "%lx-%lx", &a, &b); break; } fclose(f); }
    *lo = (char *)a; *hi = b ? (char *)b : (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *gc_stack_top(void)
{
    if (g_gc_stktop) return g_gc_stktop;
    { char *lo, *hi; gc_stack_region(&lo, &hi); g_gc_stktop = hi ? hi : (char *)&lo; }
    return g_gc_stktop;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct gc_seg_it_t { int stage; pthread_t self, mainthr; int co; scrip_coctx_t *c; char *floor; char *run_hi; } gc_seg_it_t;
static long g_gc_co_ctxs, g_gc_co_sigma, g_gc_co_parked, g_gc_shift_now, g_gc_shift_prefix;
static int g_gc_rep_pop;
static void gc_coexpr_records(void)
{
    g_gc_co_ctxs = g_gc_co_sigma = 0;
    scrip_co_gc_visit_records(&g_gc_co_ctxs, &g_gc_co_sigma);
}
static void gc_seg_begin(gc_seg_it_t *it, char *floor)
{
    it->stage = 0; it->self = pthread_self(); it->co = scrip_co_main_known(&it->mainthr); it->c = it->co ? scrip_co_gc_head() : (scrip_coctx_t *)0;
    it->floor = floor; it->run_hi = gc_stack_top(); g_gc_co_parked = 0;
}
static char *gc_seg_parked_lo(scrip_coctx_t *c, char *lo) { return (scrip_co_gc_plant() == 2 && c->park_sp && c->park_sp > lo) ? c->park_sp : lo; }
static int gc_seg_next(gc_seg_it_t *it, char **lo, char **hi, int *pop)
{
    if (it->stage == 0) { it->stage = 1;
        if (it->co && !pthread_equal(it->self, it->mainthr)) { char *slo, *shi; gc_stack_region(&slo, &shi);
            if (slo && shi && slo < shi) { *lo = gc_seg_parked_lo(scrip_co_gc_root(), slo); *hi = shi; *pop = 4; g_gc_co_parked++; return 1; } } }
    while (it->stage == 1) { scrip_coctx_t *c = it->c; char *clo, *chi;
        if (!c) { it->stage = 2; break; }
        it->c = c->gc_next;
        if (!scrip_co_stack_of(c, &clo, &chi) || clo >= chi) continue;
        if (c->alive && pthread_equal(it->self, c->thread)) { it->run_hi = chi; continue; }
        *lo = gc_seg_parked_lo(c, clo); *hi = chi; *pop = 4; g_gc_co_parked++; return 1; }
    if (it->stage == 2) { it->stage = 3; if (it->floor < it->run_hi) { *lo = it->floor; *hi = it->run_hi; *pop = 1; return 1; } }
    return 0;
}
static long gc_stack_segments(char *floor)
{
    gc_seg_it_t it; char *lo, *hi; int pop; long words = 0;
    gc_seg_begin(&it, floor);
    while (gc_seg_next(&it, &lo, &hi, &pop)) { g_gc_rep_pop = pop; words += gc_visit_segment((const char *)lo, (const char *)hi); g_gc_rep_pop = 0; }
    return words;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_dvec_ref_ok(const DESCR_t *d)
{
    rt_hblk_t *h;
    if (d->v != DT_DATA || d->slen != DATA_ELEMS_SLEN || !d->ptr) return 0;
    h = gc_blk_of((const char *)d->ptr);
    if (!h || h->type != HB_DVEC) return 0;
    return (((const char *)d->ptr - (const char *)(h + 1)) % (long)sizeof(DESCR_t)) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_block_exact(const char *q, uint16_t want_type)
{
    if (!q || (uintptr_t)q < sizeof(rt_hblk_t) || ((uintptr_t)q & 7u)) return 0;
    { rt_hblk_t *h = gc_blk_of(q); return (h && (char *)(h + 1) == q && h->type == want_type && (h->flags & HBF_TTL)) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_alloc_total(void) { return g_hp_fr.alloc_total; }
long rt_gc_alloc_str(void) { return g_hp_fr.alloc_str; }
static int  g_gc_maps_rep = -1;
#define GC_REP_POPS 5
static const char *const g_gc_rep_popname[GC_REP_POPS] = { "other", "cstack", "seam", "heapblk", "parked" };
static long g_gc_rep_ranges[GC_REP_POPS], g_gc_rep_bytes[GC_REP_POPS], g_gc_rep_agree[GC_REP_POPS], g_gc_rep_map_only[GC_REP_POPS], g_gc_rep_sniff_only[GC_REP_POPS], g_gc_rep_raw[GC_REP_POPS];
static int g_gc_rep_pop = 0;
static int gc_maps_on(void) { if (g_gc_maps_rep < 0) { const char *e = getenv("SCRIP_GC_MAPS"); g_gc_maps_rep = (e && *e && *e != '0') ? 1 : 0; } return g_gc_maps_rep; }
static int gc_maps_verbose(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_MAPS"); v = (e && *e >= '2' && *e <= '9') ? 1 : 0; } return v; }
static FILE *gc_maps_log(void) { static FILE *f = (FILE *)0; static int tried = 0; if (!tried) { const char *e = getenv("SCRIP_GC_MAPS_LOG"); tried = 1; if (e && *e) { f = fopen(e, "a"); if (f) setvbuf(f, (char *)0, _IOLBF, 0); } } return f ? f : stderr; }
static int gc_type_says_ref(const DESCR_t *d)
{
    switch (d->v) {
        case DT_S:    return 1;
        case DT_X:    return 1;
        case DT_T:    return 1;
        case DT_A:    return 1;
        case DT_DATA: return 1;
        case DT_BIG:  return 1;
        case DT_N:    return d->slen == 0 || d->slen == 2;
        default:      return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_sniff_would_take(const DESCR_t *d)
{
    if (d->v == DT_S || d->v == DT_X || (d->v == DT_N && d->slen == 0)) {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (h && d->s == (char *)(h + 1) && (d->slen == 0xFFFFFFFFu || (uint64_t)d->slen < (uint64_t)h->size)) return 1; }
    if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) return 1; }
    if (d->v == DT_N && d->slen == 2) { rt_hblk_t *vh = gc_blk_of((const char *)d->p); if (vh && vh->type == HB_AGGV && (char *)d->p == (char *)(vh + 1)) return 1; }
    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) return 1;
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN) return gc_dvec_ref_ok(d);
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_maps_report_range(const char *lo0, const char *hi0)
{
    char *lo = (char *)(((uintptr_t)lo0 + 7u) & ~(uintptr_t)7u), *hi = (char *)hi0;
    char *p;
    int k = g_gc_rep_pop;
    if (lo >= hi) return;
    g_gc_rep_ranges[k]++;
    g_gc_rep_bytes[k] += (long)(hi - lo);
    for (p = lo; p + 16 <= hi; p += 16) {
        const DESCR_t *d = (const DESCR_t *)p;
        int t = gc_type_says_ref(d), sn = gc_sniff_would_take(d);
        if (t && sn) g_gc_rep_agree[k]++;
        else if (t && !sn) g_gc_rep_map_only[k]++;
        else if (!t && sn) g_gc_rep_sniff_only[k]++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GC_WALK_LINE_CAP 48
typedef struct gc_walk_t { long frames, roots, nomap, notab, i_descr, i_heap, i_badtag, i_ptr, i_ptr_heap, i_raw, i_raw_heap, i_gap, h_words, h_cell_heap, h_raw_heap, s_words, s_cell_heap, s_raw_heap, a_words, a_heap; } gc_walk_t;
static gc_walk_t g_gw[GC_REP_POPS];
static long g_gw_lines, g_gw_suppressed;
#define GC_SPINE_REC_CAP 32
typedef struct gc_spine_rec_t { rt_hblk_t *h; const char *graph; long off; const char *const *at; } gc_spine_rec_t;
static gc_spine_rec_t g_gc_spine_rec[GC_SPINE_REC_CAP];
static int g_gc_spine_recn;
static long g_gc_spine_lost, g_gc_spine_seen;
static void gc_spine_record(const char *graph, long off, const char *const *w, rt_hblk_t *h)
{
    if (!gc_maps_on()) return;
    g_gc_spine_seen++;
    if (g_gc_spine_recn >= GC_SPINE_REC_CAP) return;
    g_gc_spine_rec[g_gc_spine_recn].h = h; g_gc_spine_rec[g_gc_spine_recn].graph = graph; g_gc_spine_rec[g_gc_spine_recn].off = off; g_gc_spine_rec[g_gc_spine_recn].at = w; g_gc_spine_recn++;
}
static int gc_tag_known(uint8_t v) { return v == DT_SNUL || v == DT_S || v == DT_I || v == DT_R || ((v & 7u) == 0 && v >= DT_P && v <= DT_MAP); }
static int gc_tag_bears_ptr(uint8_t v) { return v == DT_S || v == DT_SNUL || v == DT_X || v == DT_A || v == DT_T || v == DT_N || v == DT_DATA || v == DT_P || v == DT_PLVAR || v == DT_PLREF; }
static void gc_walk_site(const char *cls, const char *graph, long off, const char *const *w, rt_hblk_t *h)
{
    if (!gc_maps_on()) return;
    if (g_gw_lines >= GC_WALK_LINE_CAP) { g_gw_suppressed++; return; }
    g_gw_lines++;
    { char tx[25]; const unsigned char *b = (const unsigned char *)(h + 1); long n = (long)h->size - (long)sizeof(rt_hblk_t); int i = 0; for (; i < 24 && i < n; i++) tx[i] = (b[i] >= 32 && b[i] < 127) ? (char)b[i] : '.'; tx[i] = 0;
      fprintf(stderr, "[GC-WALK-%s] pop=%s graph=%s off=%ld at=%p word=%p blk=%p type=%u text=%s\n", cls, g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", off, (const void *)w, (const void *)*w, (const void *)h, (unsigned)h->type, tx); }
}
static void gc_walk_badtag(const char *graph, long off, const DESCR_t *d)
{
    if (!gc_maps_on()) return;
    if (g_gw_lines >= GC_WALK_LINE_CAP) { g_gw_suppressed++; return; }
    g_gw_lines++;
    fprintf(stderr, "[GC-WALK-BADTAG] pop=%s graph=%s off=%ld v=0x%02x slen=%u p=%p\n", g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", off, (unsigned)d->v, (unsigned)d->slen, (const void *)d->p);
}
static int gc_maps_dump(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_MAPS"); v = (e && *e >= '3' && *e <= '9') ? 1 : 0; } return v; }
#define GC_WALK_DUMP_CAP 6
static long g_gw_dumps;
static int gc_code_range(const char *w)
{
    static unsigned long lo[64], hi[64]; static int n = -1;
    if (n < 0) { FILE *f = fopen("/proc/self/maps", "r"); char ln[256]; n = 0;
        if (f) { while (fgets(ln, sizeof ln, f) && n < 64) { unsigned long a, b; char pm[8]; if (sscanf(ln, "%lx-%lx %7s", &a, &b, pm) == 3 && pm[1] == '-' && pm[2] == 'x') { lo[n] = a; hi[n] = b; n++; } } fclose(f); } }
    for (int i = 0; i < n; i++) if ((unsigned long)w >= lo[i] && (unsigned long)w < hi[i]) return 1;
    return 0;
}
static void gc_walk_dump(const char *lo, const char *base, const char *graph)
{
    if (!gc_maps_dump() || g_gw_dumps >= GC_WALK_DUMP_CAP || base - lo <= 64) return;
    g_gw_dumps++;
    fprintf(stderr, "[GC-WALK-DUMP] pop=%s graph=%s floor=%p base=%p words=%ld\n", g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", (const void *)lo, (const void *)base, (long)(base - lo) / 8);
    for (const char *p = lo; p + 8 <= base; p += 8) { const char *w = *(const char *const *)p; rt_hblk_t *h = gc_blk_of(w); const char *cls = "int"; unsigned ty = 0; int cell = (p - 8 >= lo) && gc_tag_bears_ptr(*(const uint8_t *)(p - 8));
        if (h) { cls = cell ? "heap-cell" : "heap-RAW"; ty = h->type; }
        else if (!w) cls = "nil";
        else if (gc_code_range(w)) { Dl_info di; cls = "code"; if (dladdr((const void *)w, &di) && di.dli_sname) { fprintf(stderr, "[GC-WALK-DUMP] off=%ld at=%p word=%p cls=code sym=%s+%ld\n", (long)(p - base), (const void *)p, (const void *)w, di.dli_sname, (long)(w - (const char *)di.dli_saddr)); continue; } }
        else if (w >= lo - (1L << 20) && w <= gc_stack_top()) cls = "stack";
        else if (((uintptr_t)w & 0xFFFFFF00u) == 0 && gc_tag_known((uint8_t)(uintptr_t)w)) cls = "tag";
        else if (((uintptr_t)w & 0xFFFFFF00u) == 0 && gc_tag_known((uint8_t)(uintptr_t)w) && ((uintptr_t)w >> 32) != 0) cls = "tag";
        fprintf(stderr, "[GC-WALK-DUMP] off=%ld at=%p word=%p cls=%s type=%u\n", (long)(p - base), (const void *)p, (const void *)w, cls, ty); }
}
static int gc_cell_visit(DESCR_t *d)
{
    if (d->v == DT_S || d->v == DT_X || (d->v == DT_N && d->slen == 0)) { rt_hblk_t *h = gc_blk_of(d->s);
        if (h && d->s >= (char *)(h + 1) && d->s < (char *)h + h->size) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_N && d->slen == 2) { rt_hblk_t *vh = gc_blk_of((const char *)d->p); if (vh && vh->type == HB_AGGV && (char *)d->p == (char *)(vh + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_N && d->slen != 0 && d->slen != 1 && d->slen != 2) { if (gc_blk_of(d->s)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_N && d->slen == 1) { rt_hblk_t *ch = gc_blk_of((const char *)d->ptr); if (ch && (char *)d->ptr >= (char *)(ch + 1) && (char *)d->ptr + 16 <= (char *)ch + ch->size) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) { rt_gc_visit_descr(d); return 1; }
    if (gc_dvec_ref_ok(d)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN && gc_block_exact((const char *)d->ptr, HB_DVEC)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_P || d->v == DT_PLVAR || d->v == DT_PLREF) { if (gc_blk_of((const char *)d->p)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_BIG) { rt_hblk_t *bh = gc_blk_of((const char *)d->p); if (bh && bh->type == HB_WSB && (char *)d->p == (char *)(bh + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    return 0;
}
static void gc_walk_words(const char *lo, const char *hi, int cls, const char *rlo, const char *graph, const char *base)
{
    gc_walk_t *g = &g_gw[g_gc_rep_pop];
    for (const char *p = lo; p + 8 <= hi; p += 8) { const char **w = (const char **)p; rt_hblk_t *h = gc_blk_of(*w);
        if (cls == 0) g->s_words++; else if (cls == 1) g->h_words++; else g->a_words++;
        if (cls != 2 && p + 16 <= hi && gc_cell_visit((DESCR_t *)p)) { if (gc_blk_of(*(const char *const *)(p + 8))) { if (cls == 0) g->s_cell_heap++; else g->h_cell_heap++; } p += 8; if (cls == 0) g->s_words++; else g->h_words++; continue; }
        if (!h) continue;
        if (cls == 2) { g->a_heap++; gc_walk_site("ABOVE", graph, (long)(p - base), w, h); continue; }
        if (p - 8 >= rlo && gc_tag_bears_ptr(*(const uint8_t *)(p - 8))) { if (cls == 0) g->s_cell_heap++; else g->h_cell_heap++; continue; }
        if (cls == 0) { g->s_raw_heap++; gc_walk_site("SPINE", graph, (long)(p - base), w, h); gc_spine_record(graph, (long)(p - base), w, h); } else { g->h_raw_heap++; gc_walk_site("HEADER", graph, (long)(p - base), w, h); } }
}
static long g_gc_blob_frames, g_gc_blob_entries, g_gc_blob_descr, g_gc_blob_ptr, g_gc_blob_raw, g_gc_blob_code, g_gc_blob_raw_in_heap;
static void gc_walk_interior(const char *anchor, const gc_frame_map_t *m, const char *lo, const char *hi)
{
    gc_walk_t *g = &g_gw[g_gc_rep_pop]; const uint64_t *t = (const uint64_t *)(m + 1); long n = (long)t[0]; long covered = 0; int blob = (m->flags & GC_FRAME_MAP_BLOB) ? 1 : 0;
    if (blob) g_gc_blob_frames++;
    long span = (m->flags & GC_FRAME_MAP_BLOB) ? (long)m->frame_bytes + 8 : (long)m->map_off;
    for (long i = 0; i < n; i++) { uint64_t q = t[1 + i]; int off = GC_LAY_OFF(q), size = GC_LAY_SIZE(q); unsigned kind = GC_LAY_KIND(q); const char *w = anchor + off, *e = w + size;
        if (w < lo) w = lo;
        if (e > hi) e = hi;
        if (w >= e) continue;
        covered += e - w;
        if (blob) { g_gc_blob_entries++; if (kind == GC_LAY_DESCR) g_gc_blob_descr++; else if (kind == GC_LAY_PTR_GC) g_gc_blob_ptr++; else if (kind == GC_LAY_RAW) g_gc_blob_raw++; else g_gc_blob_code++; }
        if (kind == GC_LAY_DESCR) { for (const char *c = w; c + 16 <= e; c += 16) { DESCR_t *d = (DESCR_t *)c; g->i_descr++; if (!gc_tag_known(d->v)) { g->i_badtag++; gc_walk_badtag(m->graph_name, (long)(c - anchor), d); } else { if (gc_tag_bears_ptr(d->v) && gc_blk_of((const char *)d->p)) g->i_heap++; rt_gc_visit_descr(d); } } continue; }
        for (const char *c = w; c + 8 <= e; c += 8) { const char **pw = (const char **)c; rt_hblk_t *h = gc_blk_of(*pw);
            if (kind == GC_LAY_PTR_GC) { g->i_ptr++; if (h) { g->i_ptr_heap++; rt_gc_visit_raw(pw); } continue; }
            g->i_raw++; if (h) { g->i_raw_heap++; gc_walk_site("RAW", m->graph_name, (long)(c - anchor), pw, h); if (blob && gc_type_moves(h->type)) { g_gc_blob_raw_in_heap++; if (gc_maps_on()) fprintf(stderr, "[GC-BLOB-RAW] graph=%s off=%ld kind=%u word=%p blk=%p type=%u\n", m->graph_name ? m->graph_name : "?", (long)(c - anchor), kind, (const void *)*pw, (const void *)h, (unsigned)h->type); } } } }
    if (span > covered) g->i_gap += (span - covered) / 8;
}
static int gc_walk_cell(const char *p, const char *hi, const gc_frame_map_t **mo)
{
    const DESCR_t *d = (const DESCR_t *)p; const gc_frame_map_t *m;
    if (p + 16 > hi || d->v != DT_MAP) return 0;
    m = (const gc_frame_map_t *)d->p;
    if (!m || !gc_frame_map_registered(m) || m->magic != GC_FRAME_MAP_MAGIC || m->frame_bytes != d->slen) return 0;
    *mo = m; return 1;
}
static void gc_walk_range(const char *lo0, const char *hi0)
{
    const char *lo = (const char *)(((uintptr_t)lo0 + 7u) & ~(uintptr_t)7u), *hi = hi0, *p = lo; int k = g_gc_rep_pop, nf = 0, above = 0; const char *last = "-"; gc_walk_t *g = &g_gw[k];
    while (p + 8 <= hi) {
        const char *c = p; const gc_frame_map_t *m = (const gc_frame_map_t *)0;
        while (c + 16 <= hi && !gc_walk_cell(c, hi, &m)) c += 8;
        if (!m) { if (nf == 0) g->nomap++; gc_walk_words(p, hi, above ? 2 : 0, lo, last, p); return; }
        nf++; g->frames++;
        if (gc_maps_verbose()) fprintf(gc_maps_log(), "[GC-WALK-CELL] pop=%s cell=%p graph=%s frame_bytes=%u header_bytes=%u map_off=%lu flags=%u above=%d\n", g_gc_rep_popname[k], (const void *)c, m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, (unsigned long)m->map_off, m->flags, above);
        if (m->flags & GC_FRAME_MAP_BLOB) { const char *top = c + (long)m->frame_bytes + (long)m->header_bytes; if (top > hi) top = hi;
            gc_walk_words(p, c, above ? 2 : 0, lo, above ? last : m->graph_name, c); gc_walk_interior(c + (long)m->frame_bytes, m, lo, hi); p = top; }
        else { const char *base = c - (long)m->map_off, *hlo = c + 16, *hhi = hlo + (long)m->header_bytes; if (base < p) base = p; if (hhi > hi) hhi = hi;
            { long before = g->s_raw_heap; gc_walk_words(p, base, above ? 2 : 0, lo, above ? last : m->graph_name, base); if (nf == 1 && !above && g->s_raw_heap > before) gc_walk_dump(p, base, m->graph_name); }
            if (m->flags & GC_FRAME_MAP_LAYOUT) gc_walk_interior(base, m, lo, hi); else g->notab++;
            gc_walk_words(hlo, hhi, 1, lo, m->graph_name, base); p = hhi; }
        last = m->graph_name;
        if (m->flags & GC_FRAME_MAP_ROOT) { g->roots++; { const char *q = p; const gc_frame_map_t *mn = (const gc_frame_map_t *)0; while (q + 16 <= hi && !gc_walk_cell(q, hi, &mn)) q += 8; above = mn ? 0 : 1; } }
    }
}
static long g_gc_rtccb_heap;
static void gc_walk_print(void)
{
    if (!gc_maps_on()) { memset(g_gw, 0, sizeof g_gw); g_gw_lines = 0; g_gw_suppressed = 0; return; }
    if (g_gc_rtccb_heap) fprintf(stderr, "[GC-RTCCB] heap=%ld\n", g_gc_rtccb_heap);
    for (int k = 0; k < GC_REP_POPS; k++) { gc_walk_t *g = &g_gw[k]; if (!g->frames && !g->nomap && !g->s_words && !g->a_words) continue;
        fprintf(stderr, "[GC-WALK] pop=%-7s frames=%ld roots=%ld nomap=%ld notab=%ld i_descr=%ld i_heap=%ld i_badtag=%ld i_ptr=%ld i_ptr_heap=%ld i_raw=%ld i_raw_heap=%ld i_gap=%ld h_words=%ld h_cell_heap=%ld h_raw_heap=%ld s_words=%ld s_cell_heap=%ld s_raw_heap=%ld a_words=%ld a_heap=%ld divergence=%ld\n",
            g_gc_rep_popname[k], g->frames, g->roots, g->nomap, g->notab, g->i_descr, g->i_heap, g->i_badtag, g->i_ptr, g->i_ptr_heap, g->i_raw, g->i_raw_heap, g->i_gap, g->h_words, g->h_cell_heap, g->h_raw_heap, g->s_words, g->s_cell_heap, g->s_raw_heap, g->a_words, g->a_heap, g->i_raw_heap + g->h_raw_heap + g->s_raw_heap + g->a_heap); }
    if (g_gw_suppressed) fprintf(stderr, "[GC-WALK] site lines suppressed=%ld cap=%d\n", g_gw_suppressed, GC_WALK_LINE_CAP);
    memset(g_gw, 0, sizeof g_gw); g_gw_lines = 0; g_gw_suppressed = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_spine_lost_check(void)
{
    long nlost = 0;
    if (!gc_maps_on()) { g_gc_spine_recn = 0; g_gc_spine_seen = 0; return; }
    for (int i = 0; i < g_gc_spine_recn; i++) { rt_hblk_t *h = g_gc_spine_rec[i].h; if (h->flags & HBF_MARK) continue; g_gc_spine_lost++; nlost++;
        { char tx[25]; const unsigned char *b = (const unsigned char *)(h + 1); long n = (long)h->size - (long)sizeof(rt_hblk_t); int j = 0; for (; j < 24 && j < n; j++) tx[j] = (b[j] >= 32 && b[j] < 127) ? (char)b[j] : '.'; tx[j] = 0;
          fprintf(stderr, "[GC-SPINE-LOST] graph=%s off=%ld at=%p word=%p blk=%p type=%u text=%s\n", g_gc_spine_rec[i].graph ? g_gc_spine_rec[i].graph : "-", g_gc_spine_rec[i].off, (const void *)g_gc_spine_rec[i].at, (const void *)*g_gc_spine_rec[i].at, (const void *)h, (unsigned)h->type, tx); } }
    if (nlost) fprintf(stderr, "[GC-SPINE-LOST] blocks=%ld raw_spine_words=%ld -- a word on the emitted spine that no map covers points at a block NO ROOT MARKED. ⛔ NECESSARY, NOT SUFFICIENT (cfo 2026-09-20, CFO-114): a DEAD spill slot holding a stale pointer reads identically, and 12 of the 32 green witnesses in scripts/gc_witnesses print this line while answering their oracle. It names a candidate, never a defect; only the frame map can say whether the slot is live.\n", nlost, g_gc_spine_seen);
    g_gc_spine_recn = 0; g_gc_spine_seen = 0;
}
static long gc_visit_segment(const char *lo0, const char *hi0)
{
    if (gc_maps_on()) gc_maps_report_range(lo0, hi0);
    gc_walk_range(lo0, hi0);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long g_gc_cas_bytes = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_root_cas(void)
{
    extern long rt_cas_gc_roots(void);
    g_gc_cas_bytes = rt_cas_gc_roots();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_plant_shift_bytes(void)
{
    static long v = -1;
    if (v < 0) { const char *e = getenv("SCRIP_GC_PLANT_SHIFT"); v = (e && *e) ? atol(e) : 0; if (v < 0) v = 0; v &= ~15L; if (v > (1L << 20)) v = 1L << 20; }
    if (!v) return 0;
    return ((long)(g_hp_end - g_hp_top) > v) ? v : 0;
}
static long gc_plant_shift_prefix(void)
{
    long prefix = 0; char *p = g_hp_arena;
    while (p < g_hp_top && ((rt_hblk_t *)p)->type == HB_FILL && ((rt_hblk_t *)p)->size) { prefix += ((rt_hblk_t *)p)->size; p += ((rt_hblk_t *)p)->size; }
    return prefix;
}
static long gc_collect_ex(void)
{
    extern void kw_cset_gc_roots(void); extern void core_gc_roots(void); extern void dat_gc_roots(void); extern void gen_gc_roots(void); extern void pas_gc_roots(void); extern void pl_gc_roots(void); extern void rt_gc_root_args(void); extern void rt_gc_ws_roots(void); extern void eval_gc_roots(void); extern void lower_gc_roots(void); extern void bnd_gc_roots(void); extern int rt_scan_active(void);
    char anchor; long words = 0, interior = 0; long nlive = 0, nfill = 0, before_b, after_b, n_mk = 0, n_fw = 0, n_plant = 0; char *dest; rt_hblk_t **liveo; uint64_t *livef; long li = 0; long nforeign = 0;
    long w_cnt = 0, w_idx = 0, w_pmg = 0, w_fwd = 0, w_liv = 0, w_sld = 0, w_vfy = 0, w_cel = 0, w_raw = 0, w_mov = 0; int w_tel = getenv("SCRIP_ZETA_TELEM") ? 1 : 0;
    double n_cnt = 0, n_idx = 0, n_mrk = 0, n_fwd = 0, n_liv = 0, n_sld = 0, n_vfy = 0, n_fix = 0, n_t0 = 0, n_all = w_tel ? gc_walk_ns() : 0;
    g_sxt_owner = (char *)0;
    if (g_gc_in || !g_hp_arena) return 0;
    g_gc_in = 1; before_b = (long)(g_hp_top - g_hp_arena);
    g_hp_win = (char *)0; g_hp_wend = (char *)0;
    n_t0 = w_tel ? gc_walk_ns() : 0;
    g_gc_nblk = 0;
    if (w_tel) { w_cnt = g_gc_nblk; n_cnt = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (g_gc_nblk > g_gc_icap) { g_gc_icap = g_gc_icap ? g_gc_icap : 4096; while (g_gc_icap < g_gc_nblk) g_gc_icap *= 2;
        g_gc_idxbuf = (rt_hblk_t **)gcbk_grow((void *)g_gc_idxbuf, (size_t)g_gc_icap * sizeof(*g_gc_idxbuf)); if (!g_gc_idxbuf) abort(); }
    g_gc_idx = g_gc_idxbuf;
    { char *p = g_hp_arena; long i = 0; int fold = 1; if (!g_gc_pmap) { g_gc_pmap = (uint32_t *)gcbk_alloc((((size_t)(g_hp_cap_end - g_hp_arena)) >> 9) * sizeof(uint32_t)); if (!g_gc_pmap) abort(); } while (p < g_hp_top) { rt_hblk_t *h = (rt_hblk_t *)p;
        if (fold && i >= g_gc_icap) { g_gc_icap = g_gc_icap ? g_gc_icap * 2 : 4096; g_gc_idxbuf = (rt_hblk_t **)gcbk_grow((void *)g_gc_idxbuf, (size_t)g_gc_icap * sizeof(*g_gc_idxbuf)); if (!g_gc_idxbuf) abort(); g_gc_idx = g_gc_idxbuf; }
        h->flags &= (uint16_t)~HBF_MARK;
        if (h->type == HB_ZBLK) nforeign++;
        h->fwd = 0; g_gc_idx[i] = h; { char *e = p + h->size; char *gs0 = g_hp_arena + (((size_t)(p - g_hp_arena) + 511u) & ~(size_t)511u); if (w_tel && e > gs0) w_pmg += (long)((e - gs0 + 511) >> 9);
            for (char *gs = gs0; gs < e; gs += 512) g_gc_pmap[(size_t)(gs - g_hp_arena) >> 9] = (uint32_t)i; } i++; p += h->size; } if (fold) g_gc_nblk = i; g_gc_pmap_top = g_hp_top; }
    if (w_tel) { w_idx = g_gc_nblk; n_idx = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    g_gc_mhead = (rt_hblk_t *)0; g_gc_spine_recn = 0;
    g_gc_hn = 0; if (g_gc_hs) memset(g_gc_hs, 0, (size_t)g_gc_hcap * sizeof(void *));
    g_gc_nslot = 0; g_gc_interior = 0;
    { extern void pl_tr_gc_root_ball(const char *); for (long i = 0; i < g_gc_rrng_n; i++) if (!g_gc_rrng[i].hi) pl_tr_gc_root_ball(g_gc_rrng[i].lo); }
    for (long i = 0; i < g_gc_rrng_n; i++) { if (g_gc_rrng[i].hi) continue; { const char *top = *(const char * const *)g_gc_rrng[i].lo;
        for (char *e = (char *)g_gc_rrng[i].lo + 32; e + 32 <= top; e += 32) { const char **cell = (const char **)e; DESCR_t *old = (DESCR_t *)(e + 16); if (*cell) rt_gc_visit_raw(cell); rt_gc_visit_descr(old); } } }
    rt_gc_ws_roots();
    gc_coexpr_records(); words += gc_stack_segments(g_gc_seam_sp ? g_gc_seam_sp : &anchor);
    gc_root_cas();
    kw_cset_gc_roots(); core_gc_roots(); dat_gc_roots(); gen_gc_roots(); pas_gc_roots(); pl_gc_roots(); rt_gc_root_args(); eval_gc_roots(); lower_gc_roots(); bnd_gc_roots();
    if (gc_maps_on()) for (int k = 0; k < GC_REP_POPS; k++) if (g_gc_rep_ranges[k])
        fprintf(stderr, "[GC-MAPS] pop=%-7s ranges=%ld bytes=%ld agree=%ld map_only=%ld sniff_only=%ld divergence=%ld raw_hits=%ld\n",
            g_gc_rep_popname[k], g_gc_rep_ranges[k], g_gc_rep_bytes[k], g_gc_rep_agree[k], g_gc_rep_map_only[k], g_gc_rep_sniff_only[k],
            g_gc_rep_map_only[k] + g_gc_rep_sniff_only[k], g_gc_rep_raw[k]);
    gc_walk_print();
    if (gc_maps_on()) { fprintf(stderr, "[GC-BLOB] frames=%ld entries=%ld descr=%ld ptr_gc=%ld raw=%ld code=%ld raw_in_heap=%ld\n", g_gc_blob_frames, g_gc_blob_entries, g_gc_blob_descr, g_gc_blob_ptr, g_gc_blob_raw, g_gc_blob_code, g_gc_blob_raw_in_heap);
      { long ion = 0, ioff = 0; scrip_co_gc_images(&ion, &ioff); fprintf(stderr, "[GC-COEXPR] ctxs=%ld parked=%ld sigma=%ld images_on_stack=%ld images_off_stack=%ld plant=%d\n", g_gc_co_ctxs, g_gc_co_parked, g_gc_co_sigma, ion, ioff, scrip_co_gc_plant()); } }
    g_gc_blob_frames = g_gc_blob_entries = g_gc_blob_descr = g_gc_blob_ptr = g_gc_blob_raw = g_gc_blob_code = g_gc_blob_raw_in_heap = 0;
    { static int au = -1; if (au < 0) { const char *e = getenv("SCRIP_GC_AUDIT_SLOTS"); au = (e && *e && *e != '0') ? 1 : 0; }
      if (au) { extern void gen_gc_audit_scan_slots(long *, long *); long hp = 0, un = 0; gen_gc_audit_scan_slots(&hp, &un);
        fprintf(stderr, "[GC-AUDIT] scan-save heap=%ld unrooted=%ld\n", hp, un); } }
    { extern uint64_t rtccb[32]; for (int ci = 0; ci < 32; ci++) if (gc_blk_of((const char *)rtccb[ci])) { g_gc_rtccb_heap++; if (gc_maps_on()) fprintf(stderr, "[GC-WALK-RTCCB] slot=%d word=%p\n", ci, (const void *)rtccb[ci]); } }
    if (g_gc_shield_r && *g_gc_shield_r) { extern const char *Σ; extern const char *scan_subj; if (*g_gc_shield_r == Σ || *g_gc_shield_r == scan_subj) rt_gc_visit_raw(g_gc_shield_r); else if (!g_gc_shield_r_is_probe) { fprintf(stderr, "[ZHP] rt_gc_point_arr_c refuses r0 %p: a CALLER handed this shield a word to protect and it aliases neither the subject nor scan_subj, so this shield cannot reach it and honouring the call silently would drop a root -- pass the word as a tagged DESCR cell in arr[] instead (CEO-972). The asm shim's saved-subject PROBE is rt_gc_point_arr_probe_c and is not this road.\n", (const void *)*g_gc_shield_r); abort(); } }
    { extern const char *Σ; rt_gc_visit_raw(&Σ); }
    for (int si = 0; si < g_gc_shield_n; si++) rt_gc_visit_descr(&g_gc_shield_arr[si]);
    { long walked = 0, nscan = 0, rounds = 0;
      { for (;;) { while (g_gc_mhead) { rt_hblk_t *h = g_gc_mhead; g_gc_mhead = (rt_hblk_t *)(uintptr_t)h->fwd; h->fwd = 0; walked++; nscan++;
            if (h->type == HB_DVEC) { DESCR_t *v = (DESCR_t *)(h + 1); long n = (long)(((size_t)h->size - sizeof(rt_hblk_t)) / sizeof(DESCR_t)); for (long i = 0; i < n; i++) gc_wl_push(&v[i]); continue; }
            if (h->type == HB_ARR) { ARBLK_t *a = (ARBLK_t *)(h + 1); if (gc_hins((void *)a)) gc_visit_arblk(a); continue; }
            if (h->type == HB_DINST) { DATINST_t *u = (DATINST_t *)(h + 1); if (gc_hins((void *)u)) gc_visit_datinst(u); continue; }
                        if (h->type >= HB_PLDB && h->type <= HB_PLDBK) { extern void pl_db_gc_visit(uint16_t, void *, size_t); pl_db_gc_visit(h->type, (void *)(h + 1), (size_t)h->size - sizeof(rt_hblk_t)); continue; }
            if (h->type == HB_DTP || h->type == HB_DTPRCP) { extern void pm_struct_gc_visit(uint16_t, void *, size_t); pm_struct_gc_visit(h->type, (void *)(h + 1), (size_t)h->size - sizeof(rt_hblk_t)); continue; }
            if (h->type == HB_PVEC) { const char **v = (const char **)(h + 1); long n = (long)(((size_t)h->size - sizeof(rt_hblk_t)) / sizeof(void *)); for (long i = 0; i < n; i++) if (v[i]) rt_gc_visit_raw(&v[i]); continue; }
            if (h->type == HB_AGGV) { gc_visit_vcell((VCELL_t *)(h + 1)); continue; }
            if (h->type == HB_AGGB) continue;
            if (h->type == HB_AGGP) { TBPAIR_t *e = (TBPAIR_t *)(h + 1); if (e->key) gc_mark_agg(e->key);
                rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val); continue; }
            if (h->type == HB_AGGT) { struct _TBBLK_t *t = (struct _TBBLK_t *)(h + 1); if (gc_hins((void *)t)) gc_visit_tbblk(t); continue; }
            if (h->type < HB_ZCOL || h->type == HB_FILL || h->type == HB_ZBLK || h->type == HB_WSC || h->type == HB_WSB) continue;
            interior += (long)(((size_t)h->size - sizeof(rt_hblk_t)) / sizeof(void *)); }
        if (g_gc_wln == 0) break;
        rounds++; g_gc_wl_draining = 1; while (g_gc_wln > 0) gc_visit_one(g_gc_wl[--g_gc_wln]); g_gc_wl_draining = 0; } }
      if (w_tel) { n_mrk = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); fprintf(stderr, "[ZGC-MARK] arm=%s titles-walked=%ld blocks-scanned=%ld rounds=%ld nblk=%ld\n", "WL", walked, nscan, rounds, g_gc_nblk); n_t0 = gc_walk_ns(); }
    }
    gc_spine_lost_check();
    { static int cov = -1; if (cov < 0) { const char *e = getenv("SCRIP_GC_COVERAGE"); cov = (e && *e && *e != '0') ? 1 : 0; }
      if (cov) fprintf(stderr, "[GC-COV] ranges=%ld cas_scanned_bytes=%ld words_scanned=%ld interior_words=%ld\n", g_gc_rrng_n, g_gc_cas_bytes, words, interior); }
    { long shift = gc_plant_shift_bytes(); g_gc_shift_prefix = shift ? gc_plant_shift_prefix() : 0; dest = g_hp_arena + g_gc_shift_prefix + shift; g_gc_shift_now = shift; }
    { int fold = 1;
    if (fold) { gc_live_grow(0); liveo = g_gc_liveo; livef = g_gc_livef; }
    for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
        if (h->flags & HBF_MARK) { n_mk++; if (gc_plant_pin_type() < 0 || (long)h->type == gc_plant_pin_type()) n_plant++; if (n_plant == gc_plant_pin_skip()) { h->fwd = 0; dest += h->size; }
            else { h->fwd = (uint64_t)dest; dest += h->size; nlive++; } }
        else h->fwd = 0;
        if (h->fwd) n_fw++;
        if (fold && h->fwd) { if (li >= g_gc_lcap) { gc_live_grow(li); liveo = g_gc_liveo; livef = g_gc_livef; } liveo[li] = h; livef[li] = h->fwd; li++; } }
    if (w_tel) { w_fwd = g_gc_nblk; n_fwd = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); } }
    if (n_mk != n_fw) fprintf(stderr, "[ZGC-PIN] VIOLATION marked=%ld forwarded=%ld skipped=%ld -- a marked block was not given a forwarding address, so it keeps its address while the heap slides around it: that is PINNING under another name, and no pinning mechanism returns in any form (Lon 2026-09-17, CEO-831)\n", n_mk, n_fw, n_mk - n_fw);
    for (long i = 0; i < g_gc_nslot; i++) { gc_slot_t *sl = &g_gc_slots[i]; const char **loc = sl->hloc ? (const char **)((char *)(sl->hloc + 1) + sl->off) : (const char **)sl->off;
        rt_hblk_t *h = gc_blk_of(*loc); if (h && h->fwd && h->fwd != (uint64_t)h) *loc = (const char *)((rt_hblk_t *)h->fwd + 1) + (*loc - (const char *)(h + 1)); }
    if (w_tel) { w_cel = g_gc_nslot; w_raw = 0; n_fix = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (w_tel) n_t0 = gc_walk_ns();
    dest = g_hp_arena;
    if (g_gc_shift_now) {
        for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] < (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        for (long i = li - 1; i >= 0; i--) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] > (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        { rt_hblk_t *fl = (rt_hblk_t *)(g_hp_arena + g_gc_shift_prefix); fl->fwd = 0; fl->size = (uint32_t)g_gc_shift_now; fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
          for (char *q = g_hp_arena; q < g_hp_arena + g_gc_shift_prefix; q += ((rt_hblk_t *)q)->size) nfill++; }
        dest = li ? (char *)livef[li - 1] + ((rt_hblk_t *)livef[li - 1])->size : g_hp_arena + g_gc_shift_prefix + g_gc_shift_now;
        if (w_tel || gc_maps_on()) fprintf(stderr, "[GC-SHIFT] plant: every live block forwarded %ld bytes up, over %ld bytes of kept fill at the arena start -- a stale copy of any heap address is wrong after every collection\n", g_gc_shift_now, g_gc_shift_prefix + g_gc_shift_now);
    } else
    for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size;
        if ((char *)livef[i] == (char *)h) { if (dest < (char *)h) { rt_hblk_t *fl = (rt_hblk_t *)dest; fl->fwd = 0; fl->size = (uint32_t)((char *)h - dest); fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
            if ((long)fl->size > (long)(g_hp_wend - g_hp_win)) { g_hp_win = dest; g_hp_wend = (char *)h; } } dest = (char *)h + sz; }
        else { memmove((void *)livef[i], (void *)h, (size_t)sz); dest = (char *)livef[i] + sz; if (w_tel) w_mov += (long)sz; } }
    g_hp_top = dest; g_hp_blocks = nlive + nfill;
    for (long i = 0; i < li; i++) { rt_hblk_t *nh = (rt_hblk_t *)livef[i]; nh->fwd = 0; nh->flags = (uint16_t)((nh->flags | HBF_TTL) & ~HBF_MARK); }
    after_b = (long)(g_hp_top - g_hp_arena);
    { static int psn = -1; if (psn < 0) { const char *e = getenv("SCRIP_GC_POISON"); psn = (e && *e) ? (*e != '0') : 1; } if (psn) { char *ot = g_hp_arena + before_b; long pb = ot > g_hp_top ? (long)(ot - g_hp_top) : 0; if (pb > 0) memset(g_hp_top, 0xDB, (size_t)pb); if (w_tel) fprintf(stderr, "[ZGC-POISON] vacated=%ldB filled=0xDB top=%p oldtop=%p\n", pb, (void *)g_hp_top, (void *)ot); } }
    if (w_tel) { w_sld = li; n_sld = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    { extern void rt_nv_memo_invalidate(void); rt_nv_memo_invalidate(); }
    rt_gcheap_verify();
    if (w_tel) { w_vfy = nlive + nfill; n_vfy = gc_walk_ns() - n_t0; }
    g_gc_runs++;
    if (w_tel) fprintf(stderr, "[ZGC-WALK] arm=%s nblk=%ld | count=%ld/%.0fus index=%ld/%.0fus pmap-gran=%ld fwd=%ld/%.0fus live=%ld/%.0fus | mark=%.0fus fixup=%ld+%ld/%.0fus slide=%ld/%.0fus moved=%ldB verify=%ld/%.0fus | walk-floor=%ld titles %.0fus of %.0fus total\n",
        "FOLD", g_gc_nblk, w_cnt, n_cnt / 1e3, w_idx, n_idx / 1e3, w_pmg, w_fwd, n_fwd / 1e3, w_liv, n_liv / 1e3, n_mrk / 1e3, w_cel, w_raw, n_fix / 1e3, w_sld, n_sld / 1e3, w_mov, w_vfy, n_vfy / 1e3,
        w_cnt + w_idx + w_fwd + w_liv + w_vfy, (n_cnt + n_idx + n_fwd + n_liv + n_vfy) / 1e3, (gc_walk_ns() - n_all) / 1e3);
    if (g_gc_dvec_elems && (w_tel || gc_maps_on())) fprintf(stderr, "[GC-DVEC] elems=%ld non_dvec=%ld\n", g_gc_dvec_elems, g_gc_dvec_nondvec);
    if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZGC] regeneration #%ld (%s): blocks %ld->%ld (fill %ld) bytes %ld->%ld reclaimed %ld win=%ld slots=%ld interior=%ld wl_depth_max=%ld marked=%ld forwarded=%ld\n", g_gc_runs, "E", g_gc_nblk, nlive, nfill, before_b, after_b, before_b - after_b, (long)(g_hp_wend - g_hp_win), g_gc_nslot, g_gc_interior, g_gc_wlmax, n_mk, n_fw);
    g_hp_gcline = g_hp_top + gc_line_span((long)((g_hp_end - g_hp_top) >> 1));
    g_hp_fr.line = gc_line_paced() ? g_hp_gcline : g_hp_end;
    g_gc_idx = (rt_hblk_t **)0;
    g_gc_in = 0;
    return before_b - after_b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_collect_c(char *floor)
{
    long r;
    g_gc_seam_sp = floor;
    r = gc_collect_ex();
    g_gc_seam_sp = (char *)0;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_poll(void)
{
    char here;
    if (g_gc_in) return;
    if (!g_gc_pending && !(g_hp_gcline && g_hp_top > g_hp_gcline)) return;
    g_gc_polls++;
    rt_gc_point_arr_c((DESCR_t *)0, 0, (const char **)0, &here);
}
long rt_gc_polls_count(void) { return g_gc_polls; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_free(void) { if (!g_hp_arena) rt_gcheap_init(); return (long)(g_hp_end - g_hp_top); }
long rt_gc_runs_count(void) { return g_gc_runs; }
