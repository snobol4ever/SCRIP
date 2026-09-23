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
#define GC_HEAP_MB 1
#define GC_HEAP_KB 128
#define GC_RESERVE_FLOOR_MB 512
#define GC_HEAP_CAP_KB 4096
#define GC_HEAP_KB_FLOOR 64
#include "descr.h"
#include "pin_va.h"
#include "gc_frame_map.h"
#include "gc_audit_b.h"
#include "../rtx/rtcc.h"
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
static long g_hp_capped = 0;
static long g_hp_grown = 0;
static long g_hp_grows = 0;
static long g_hp_live = 0;
static int   g_hp_report_reg = 0;
static void gc_static_segs_init(void);
static void gc_birth_record(char *at, uint64_t total, uint16_t type, void *ra_site, void *ra_from);
typedef struct gc_vac_t { char *at; char *fwd; uint32_t size; uint32_t gen; uint16_t type; uint16_t pad; long serial; void *ra_site; void *ra_from; } gc_vac_t;
static gc_vac_t *g_gc_vac = (gc_vac_t *)0;
static long g_gc_vacn = 0, g_gc_vaccap = 0, g_gc_vacover = 0, g_gc_vacgen = 0;
static gc_vac_t *gc_vac_ledger(void);
static int gc_birth_on(void);
static char *g_hp_qlo = (char *)0;
static char *g_hp_qhi = (char *)0;
static long g_hp_qgen = 0, g_hp_qarm = 0, g_hp_qbytes = 0;
static void gc_quar_release(char *need_end);
static void gc_quar_arm(char *lo);
int g_gc_pending;
static long g_gc_polls = 0;
static const char *g_gc_top_graph = (const char *)0;
static const char *g_gc_poll_site = (const char *)0;
extern long g_line;
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
        gc_quar_release(g_hp_top + d);
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
    long win_mb = (long)(g_hp_chunk >> 20), rsv_mb = (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 20) : 0L;
    long win_kb = (long)(g_hp_chunk >> 10);
    { const char *x = getenv("SCRIP_GC_EXERCISE"); const char *st = getenv("SCRIP_GC_STRESS");
      if (x && *x && *x != '0') fprintf(stderr, "[GC-EXERCISE] arena_kb=%ld arena_mb=%ld reserve_mb=%ld stress=%s collections=%ld blocks=%ld bytes=%ld capped=%ld grew=%ld cap_kb=%ld grows=%ld\n", win_kb, win_mb, rsv_mb, (st && *st) ? st : "0", rt_gc_runs_count(), g_hp_blocks, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L, g_hp_capped, (long)(g_hp_grown >> 10), (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 10) : 0L, g_hp_grows); }
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    long live = rt_gcheap_verify();
    fprintf(stderr, "[ZHP] arena=%ldKB reserve=%ldMB collections=%ld blocks=%ld(alloc'd)=%ld(walked) bytes=%ld capped=%ld grew=%ldKB cap=%ldKB grows=%ld verify=OK\n", win_kb, rsv_mb, rt_gc_runs_count(), g_hp_blocks, live, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L, g_hp_capped, (long)(g_hp_grown >> 10), (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 10) : 0L, g_hp_grows);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_huge_advise(char *a0, char *e0)
{
    const char *nh = getenv("SCRIP_NOHUGE");
    if (nh && *nh && *nh != '0') return;
    { uintptr_t a = ((uintptr_t)a0 + 0x1FFFFFu) & ~(uintptr_t)0x1FFFFFu, e = ((uintptr_t)e0) & ~(uintptr_t)0x1FFFFFu; if (e > a) madvise((void *)a, (size_t)(e - a), MADV_HUGEPAGE); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_cap_hit(uint64_t need);
_Static_assert(sizeof(long) == 8, "THE LINE FOLLOWS THE TOP AND THE WINDOW FOLLOWS THE LIVE SET (cto, 2026-09-23, row gc-heap-growth-...; hq_icon's geddump measured 128 KB timing out past 15 s against 0.115 s at the 4096 KB cap): the collection line is recomputed as top plus min(SCRIP_GC_LINE_MB, half of what is free) at init, after every grow and after every collection -- a line fixed at half the STARTING window made every poll collect once the live set outgrew it -- and a grow commits the larger of the request and the committed size whenever the last collection left the window more than half live, else the request alone; still lazy, still one mprotect per grow, still nothing past the cap");
static void rt_gcheap_line_reset(void)
{
    long half = (long)((size_t)(g_hp_end - g_hp_top) >> 1);
    g_hp_gcline = g_hp_top + gc_line_span(half);
    g_hp_fr.line = gc_line_paced() ? g_hp_gcline : g_hp_end;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_gcheap_grow(uint64_t need)
{
    size_t left = (size_t)(g_hp_cap_end - g_hp_end);
    if (!left) { rt_gcheap_cap_hit(need); return 0; }
    { size_t want = (size_t)need + sizeof(rt_hblk_t), committed = (size_t)(g_hp_end - g_hp_arena); want = (want + 0xFFFu) & ~(size_t)0xFFFu;
      if ((size_t)g_hp_live * 2u > committed && want < committed) want = committed;
      if (want > left) want = left;
      if (mprotect(g_hp_end, want, PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZHP] lazy commit refused %ld KB at %p under a %ld KB hard cap\n", (long)(want >> 10), (void *)g_hp_end, (long)((g_hp_cap_end - g_hp_arena) >> 10)); return 0; }
      gc_huge_advise(g_hp_end, g_hp_end + want);
      g_hp_end += want;
      g_hp_grown += (long)want; g_hp_grows++;
      rt_gcheap_line_reset();
      if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZHP] lazy commit -> %ld KB of a %ld KB hard cap (committed %ld KB total)\n", (long)((g_hp_end - g_hp_arena) >> 10), (long)((g_hp_cap_end - g_hp_arena) >> 10), (long)(g_hp_grown >> 10));
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_cap_hit(uint64_t need)
{
    if (g_hp_capped++) return;
    fprintf(stderr, "[ZHP] HARD CAP REACHED: %ld KB is committed, THE CAP IS %ld KB AND THE HEAP DOES NOT EXTEND PAST IT (Lon 2026-09-21, in-chat to the ceo, verbatim: \"Place a hard cap on the GC HEAP. Do not extend it.\" and \"Do however use the lazy instantiation of memory as the heap grows.\"; ceo CEO-1101). Memory inside the cap is instantiated LAZILY, a page at a time, as the heap grows into it; there is NOTHING BEYOND THE CAP -- the old 8x reserve is what let a point DECLARED at 64 KB grow to 2 MB and read as a 2 MB measurement wearing a 64 KB label (ceo CEO-1100, coo COO-139). A collection is ARMED for the next safe point and this request wanted %llu bytes; if the run now dies exhausted, read the distance to the next SAFE POINT before reading the size of the cap.\n", (long)((g_hp_end - g_hp_arena) >> 10), (long)((g_hp_cap_end - g_hp_arena) >> 10), (unsigned long long)need);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_init(void)
{
    long mb = (long)GC_HEAP_MB, cap_mb, cap_kb, kb = (long)GC_HEAP_KB;
    { const char *e = getenv("SCRIP_HEAP_MB"); if (e && *e) { long v = atol(e); if (v >= 1 && v <= 4096) { mb = v; kb = v * 1024L; } } }
    { const char *e = getenv("SCRIP_HEAP_KB"); if (e && *e) { long v = atol(e);
        if (v < (long)GC_HEAP_KB_FLOOR || v > 4096L * 1024L) { fprintf(stderr, "[ZHP] SCRIP_HEAP_KB=%ld is outside %d..%ld KB and is REFUSED -- the window is not silently clamped, because a run that grades at a size it was not asked for is a false reading (ceo CEO-1095)\n", v, (int)GC_HEAP_KB_FLOOR, 4096L * 1024L); abort(); }
        kb = v; } }
    cap_kb = (long)GC_HEAP_CAP_KB; if (cap_kb < kb) cap_kb = kb;
    { const char *e = getenv("SCRIP_HEAP_MAX_MB"); if (e && *e) { long v = atol(e); cap_kb = (v * 1024L >= kb) ? v * 1024L : kb; } }
    { const char *e = getenv("SCRIP_HEAP_CAP_KB"); if (e && *e) { long v = atol(e); if (v < kb) { fprintf(stderr, "[ZHP] SCRIP_HEAP_CAP_KB=%ld is BELOW the %ld KB window and is REFUSED -- a cap under its own window is not a smaller heap, it is an unstatable one (ceo CEO-1101)\n", v, kb); abort(); } cap_kb = v; } }
    cap_mb = (cap_kb + 1023L) / 1024L; if (cap_mb < 1) cap_mb = 1;
    { size_t rsv = (((size_t)cap_kb << 10) + 0xFFFu) & ~(size_t)0xFFFu;
      void *rv = mmap((void *)0, rsv, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
      if (rv == MAP_FAILED) { fprintf(stderr, "[ZHP] heap mmap failed at the %ld KB hard cap\n", cap_kb); abort(); }
      g_hp_arena = (char *)rv; g_hp_cap_end = g_hp_arena + rsv; }
    g_hp_chunk = (((size_t)kb << 10) + 4095u) & ~(size_t)4095u;
    if (mprotect(g_hp_arena, g_hp_chunk, PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZHP] heap window commit failed (%ld KB of a %ld MB reserve)\n", kb, cap_mb); abort(); }
    g_hp_top = g_hp_arena; g_hp_end = g_hp_arena + g_hp_chunk;
    gc_huge_advise(g_hp_arena, g_hp_end);
    g_hp_virgin = g_hp_arena;
    rt_gcheap_line_reset();
    gc_static_segs_init();
    { const char *b = getenv("SCRIP_GC_BIRTH_LEDGER"); if (b && *b && *b != '0') (void)gc_vac_ledger(); }
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
    gc_birth_record(at, total, type, __builtin_return_address(1), __builtin_return_address(2));
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
    if (g_alloc_detax == 1 && g_ah_on <= 0) { uint64_t tf = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull); if (gc_line_paced() && g_hp_gcline && !g_gc_in && g_hp_top + tf > g_hp_gcline) g_gc_pending = 1; if (g_hp_top + tf <= g_hp_end) { if (g_hp_qlo) gc_quar_release(g_hp_top + tf); { void *rf = rt_gcheap_carve(g_hp_top, tf, type); g_hp_top += tf; return rf; } } }
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
      if (!g_alloc_detax) g_alloc_detax = (stress_n == 0 && budget == 0 && g_ah_on <= 0 && !gc_birth_on() && g_hp_arena && g_hp_report_reg) ? 1 : -1;
      g_hp_fr.armed = (g_alloc_detax == 1 && g_ah_on <= 0 && !gc_birth_on()) ? 1 : 0;
      if (budget) { since += (long)total; if (since >= budget && (g_hp_top - g_hp_arena) * 2 >= (g_hp_end - g_hp_arena)) { since = 0; g_gc_pending = 2; } } }
    if (gc_line_paced() && g_hp_gcline && !g_gc_in && g_hp_top + total > g_hp_gcline && g_hp_top + total <= g_hp_end) g_gc_pending = 1;
    if (g_hp_top + total > g_hp_end && g_hp_win + total > g_hp_wend) { g_gc_pending = 1; rt_gcheap_grow(total); }
    if (g_hp_top + total <= g_hp_end) { if (g_hp_qlo) gc_quar_release(g_hp_top + total); r = rt_gcheap_carve(g_hp_top, total, type); g_hp_top += total; return r; }
    if (g_hp_win + total <= g_hp_wend) {
        uint64_t avail = (uint64_t)(g_hp_wend - g_hp_win);
        if (avail - total == sizeof(rt_hblk_t)) total += sizeof(rt_hblk_t);
        r = rt_gcheap_carve(g_hp_win, total, type);
        g_hp_win += total;
        if (g_hp_win < g_hp_wend) { rt_hblk_t *fl = (rt_hblk_t *)g_hp_win; fl->fwd = 0; fl->size = (uint32_t)(g_hp_wend - g_hp_win); fl->type = HB_FILL; fl->flags = HBF_TTL; }
        return r;
    }
    fprintf(stderr, "[ZHP] heap exhausted AT THE HARD CAP (%ld KB cap, %ld KB committed lazily so far, %ld blocks live, cap hit %ld times, COLLECTIONS RUN %ld) -- THIS REQUEST wanted %llu payload bytes (%llu with the header) of block kind %u. THE HEAP DOES NOT EXTEND ON LON'S ORDER, so this is one of exactly three things and never a fourth: a CORRUPTED LENGTH (a size computed from a pointer a collection moved asks for the world -- read the size FIRST), a MISSING SAFE POINT between this allocation and the last one (the collection is armed and never ran), or a live set that genuinely does not fit the window THIS RUN NAMED. SCRIP_HEAP_MAX_MB and SCRIP_HEAP_CAP_KB name the CAP ITSELF rather than a reserve above the window. \342\233\224 READ COLLECTIONS RUN FIRST: AN ABORT WITH ZERO COLLECTIONS IS NEVER A CAPACITY VERDICT -- nothing was ever reclaimed, so the cap was never full of LIVE data, and the cause is a MISSING SAFE POINT between two allocations. Capacity means the collector RAN, reclaimed what it could, and the survivors still did not fit (ceo CEO-1101, the classification the coo's item-4 rail buckets on).\n", (long)((g_hp_cap_end - g_hp_arena) >> 10), (long)((g_hp_end - g_hp_arena) >> 10), g_hp_blocks, g_hp_capped, rt_gc_runs_count(), (unsigned long long)payload_bytes, (unsigned long long)total, (unsigned)type);
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
static size_t gc_pg(void)
{
    static size_t pg = 0;
    if (!pg) { long v = sysconf(_SC_PAGESIZE); pg = (v > 0) ? (size_t)v : (size_t)4096; }
    return pg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_trap_on(void)
{
    static int t = -1;
    if (t < 0) { const char *e = getenv("SCRIP_GC_TRAP"); t = (e && *e) ? (*e != '0') : 1; }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_quar_release(char *need_end)
{
    size_t pg; char *b;
    if (!g_hp_qlo || need_end <= g_hp_qlo) return;
    pg = gc_pg(); b = (char *)(((uintptr_t)need_end + (pg - 1)) & ~(uintptr_t)(pg - 1));
    if (b > g_hp_qhi) b = g_hp_qhi;
    if (b <= g_hp_qlo) return;
    if (mprotect(g_hp_qlo, (size_t)(b - g_hp_qlo), PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZGC-TRAP] could not re-commit vacated ground arena+%ld..arena+%ld for the allocator -- the stale-read trap refuses the run rather than let the mutator write ground the kernel will not hand back\n", (long)(g_hp_qlo - g_hp_arena), (long)(b - g_hp_arena)); abort(); }
    g_hp_qlo = b;
    if (g_hp_qlo >= g_hp_qhi) { g_hp_qlo = (char *)0; g_hp_qhi = (char *)0; g_gc_vacn = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_quar_arm(char *lo)
{
    size_t pg; char *a; char *b; char *ceil;
    if (!gc_trap_on() || !g_hp_arena) return;
    pg = gc_pg();
    ceil = g_hp_virgin < g_hp_end ? g_hp_virgin : g_hp_end;
    a = (char *)(((uintptr_t)lo + (pg - 1)) & ~(uintptr_t)(pg - 1));
    b = (char *)((uintptr_t)ceil & ~(uintptr_t)(pg - 1));
    if (g_hp_qhi > b) b = g_hp_qhi;
    if (b <= a) return;
    if (mprotect(a, (size_t)(b - a), PROT_NONE) != 0) { static int said = 0; if (!said) { said = 1; fprintf(stderr, "[ZGC-TRAP] mprotect PROT_NONE refused arena+%ld..arena+%ld -- THE STALE-READ TRAP IS OFF FOR THIS RUN and a stale read will return a plausible 0xDB answer again; a silent instrument is never evidence of health\n", (long)(a - g_hp_arena), (long)(b - g_hp_arena)); } g_hp_qlo = (char *)0; g_hp_qhi = (char *)0; return; }
    g_hp_qlo = a; g_hp_qhi = b; g_hp_qgen = g_gc_vacgen; g_hp_qarm++; g_hp_qbytes += (long)(b - a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_birth_on(void) { return (g_gc_vac && g_gc_vac[g_gc_vaccap].size) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static gc_vac_t *gc_vac_ledger(void)
{
    if (!g_gc_vac) { const char *e = getenv("SCRIP_GC_TRAP_LEDGER"); const char *k = getenv("SCRIP_GC_BIRTH_LEDGER"); long c = e && *e ? atol(e) : 65536; long b = k && *k ? atol(k) : 0;
        if (c < 64) c = 64; if (c > (1L << 22)) c = 1L << 22; if (b < 0) b = 0; if (b > (1L << 22)) b = 1L << 22;
        g_gc_vac = (gc_vac_t *)gcbk_alloc((size_t)(c + b + 1) * sizeof(gc_vac_t)); if (!g_gc_vac) return (gc_vac_t *)0; g_gc_vaccap = c;
        { gc_vac_t *hd = &g_gc_vac[c]; hd->at = (char *)0; hd->fwd = (char *)0; hd->size = (uint32_t)b; hd->gen = 0; hd->type = 0; hd->pad = 0; hd->serial = 0; hd->ra_site = (void *)0; hd->ra_from = (void *)0; } }
    return g_gc_vac;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_birth_record(char *at, uint64_t total, uint16_t type, void *ra_site, void *ra_from)
{
    gc_vac_t *hd; long b, s;
    if (!g_gc_vac) return;
    hd = &g_gc_vac[g_gc_vaccap]; b = (long)hd->size; if (b <= 0) return;
    s = hd->serial++;
    { gc_vac_t *v = &g_gc_vac[g_gc_vaccap + 1 + (s % b)];
      v->at = at; v->fwd = (char *)0; v->size = (uint32_t)total; v->gen = 0; v->type = type; v->pad = 0; v->serial = s; v->ra_site = ra_site; v->ra_from = ra_from; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_birth_find(char *at)
{
    long i, b;
    if (!g_gc_vac || !at) return -1;
    b = (long)g_gc_vac[g_gc_vaccap].size; if (b <= 0) return -1;
    { long best = -1;
      for (i = 1; i <= b; i++) { gc_vac_t *v = &g_gc_vac[g_gc_vaccap + i];
          if (v->gen == 0 && v->size && v->at == at && (best < 0 || v->serial > g_gc_vac[best].serial)) best = g_gc_vaccap + i; }
      return best; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_birth_cover(char *f)
{
    long i, b, best = -1;
    if (!g_gc_vac || !f) return -1;
    b = (long)g_gc_vac[g_gc_vaccap].size; if (b <= 0) return -1;
    for (i = 1; i <= b; i++) { gc_vac_t *v = &g_gc_vac[g_gc_vaccap + i];
        if (v->gen == 0 && v->size && f >= v->at && f < v->at + (long)v->size && (best < 0 || v->serial > g_gc_vac[best].serial)) best = g_gc_vaccap + i; }
    return best;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *gc_chain_back(long hit, long *hops)
{
    long i, cur = hit, k = 0; char *a = g_gc_vac[hit].at;
    for (;;) { long prev = -1;
        for (i = g_gc_vacn - 1; i >= 0; i--) if (g_gc_vac[i].fwd == a && g_gc_vac[i].gen < g_gc_vac[cur].gen) { prev = i; break; }
        if (prev < 0 || k >= 4096) break;
        cur = prev; a = g_gc_vac[cur].at; k++; }
    *hops = k; return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_vac_record(char *vlo)
{
    long i, need = 0;
    if (!gc_trap_on()) return;
    if (!gc_vac_ledger()) return;
    for (i = 0; i < g_gc_nblk; i++) if ((char *)g_gc_idx[i] >= vlo) need++;
    if (need > g_gc_vaccap - g_gc_vacn) { g_gc_vacn = 0; g_gc_vacover++; }
    g_gc_vacgen = g_gc_runs + 1;
    for (i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
        if ((char *)h < vlo) continue;
        if (g_gc_vacn >= g_gc_vaccap) break;
        { gc_vac_t *v = &g_gc_vac[g_gc_vacn++]; v->at = (char *)h; v->fwd = h->fwd ? (char *)(uintptr_t)h->fwd : (char *)0; v->size = h->size; v->gen = (uint32_t)g_gc_vacgen; v->type = h->type; v->pad = 0; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_gc_stale_addr_report(void *fault, void *ip)
{
    char bf[4096]; int n = 0; char *f = (char *)fault; long off, i, hit = -1;
    if (!g_hp_arena || f < g_hp_arena || f >= g_hp_cap_end) return 0;
    off = (long)(f - g_hp_arena);
    n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE] SIGSEGV touching GC heap ground at %p (arena+%ld) from instruction %p -- a STALE HEAP POINTER was used, not a wild address\n", fault, off, ip);
    if (g_hp_qlo && f >= g_hp_qlo && f < g_hp_qhi) {
        n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   this page is QUARANTINED PROT_NONE: collection #%ld (or an earlier one) vacated arena+%ld..arena+%ld and no allocation has taken it back; the live top is arena+%ld\n", g_hp_qgen, (long)(g_hp_qlo - g_hp_arena), (long)(g_hp_qhi - g_hp_arena), (long)(g_hp_top - g_hp_arena));
        for (i = g_gc_vacn - 1; i >= 0; i--) if (f >= g_gc_vac[i].at && f < g_gc_vac[i].at + g_gc_vac[i].size) { hit = i; break; }
        if (hit >= 0 && g_gc_vac[hit].fwd) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   the block that lived here: #%ld kind=%u size=%u at arena+%ld, +%ld into it -- it was MOVED to arena+%ld by collection #%ld, so the holder of this pointer was NEVER VISITED and kept the pre-move address\n", hit, (unsigned)g_gc_vac[hit].type, (unsigned)g_gc_vac[hit].size, (long)(g_gc_vac[hit].at - g_hp_arena), (long)(f - g_gc_vac[hit].at), (long)(g_gc_vac[hit].fwd - g_hp_arena), (long)g_gc_vac[hit].gen);
        else if (hit >= 0) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   the block that lived here: #%ld kind=%u size=%u at arena+%ld, +%ld into it -- it was RECLAIMED by collection #%ld because nothing marked it, so the holder of this pointer was never visited either\n", hit, (unsigned)g_gc_vac[hit].type, (unsigned)g_gc_vac[hit].size, (long)(g_gc_vac[hit].at - g_hp_arena), (long)(f - g_gc_vac[hit].at), (long)g_gc_vac[hit].gen);
        else n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   no block in the vacated ledger covers this address (ledger holds %ld entries through collection #%ld, %ld resets) -- this ground was vacated before the ledger's oldest entry, so the block is not nameable from here\n", g_gc_vacn, g_gc_vacgen, g_gc_vacover);
        { long hops = 0; char *born = (char *)0; long bk = -1; int viachain = 0;
            if (hit >= 0) { born = gc_chain_back(hit, &hops); bk = gc_birth_find(born); viachain = bk >= 0; }
            if (bk < 0) { bk = gc_birth_cover(f); if (bk >= 0) { born = g_gc_vac[bk].at; hops = 0; } }
            if (bk >= 0) { Dl_info d0, d1; gc_vac_t *v = &g_gc_vac[bk];
                int k0 = dladdr(v->ra_site, &d0), k1 = v->ra_from ? dladdr(v->ra_from, &d1) : 0;
                const char *s0 = (k0 && d0.dli_sname) ? d0.dli_sname : "?"; const char *s1 = (k1 && d1.dli_sname) ? d1.dli_sname : "?";
                const char *m0 = (k0 && d0.dli_fname) ? d0.dli_fname : "?"; const char *m1 = (k1 && d1.dli_fname) ? d1.dli_fname : "?";
                long f0 = k0 ? (long)((char *)v->ra_site - (char *)d0.dli_fbase) : 0L, f1 = k1 ? (long)((char *)v->ra_from - (char *)d1.dli_fbase) : 0L;
                n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-BIRTH]   block #%ld, kind=%u/%s, size=%u, allocated by %s (%s+0x%lx) from %s (%s+0x%lx), born at arena+%ld, +%ld into it, %s\n",
                    v->serial, (unsigned)v->type, HB_KIND_NAME(v->type), (unsigned)v->size, s0, m0, f0, s1, m1, f1, (long)(born - g_hp_arena), (long)(f - born),
                    viachain ? "reached by walking the vacated ledger back through its relocations" : "matched by address in the birth ring, with no vacated-ledger entry to relocate it");
                if (viachain) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-BIRTH]   it was relocated %ld time(s) between that birth and the collection that vacated the ground you just read\n", hops); }
            else if (!gc_birth_on()) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-BIRTH]   THE BIRTH LEDGER IS OFF, so this fault names an address and not a defect -- set SCRIP_GC_BIRTH_LEDGER=<ring entries> and the report will name the block's serial, its type at birth and the site that allocated it\n");
            else n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-BIRTH]   the birth ledger does not hold this block: the chain walked back %ld relocation(s) to arena+%ld and no birth record survives there (ring holds %ld entries, %ld births so far) -- raise SCRIP_GC_BIRTH_LEDGER or the record has been overwritten\n", hops, born ? (long)(born - g_hp_arena) : -1L, (long)g_gc_vac[g_gc_vaccap].size, g_gc_vac[g_gc_vaccap].serial); }
    } else if (f >= g_hp_top) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   the address is above the live top (arena+%ld) but its page is not quarantined: vacated ground the allocator has already taken back, or ground beyond the committed window (arena+%ld)\n", (long)(g_hp_top - g_hp_arena), (long)(g_hp_end - g_hp_arena));
    else n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   the address is BELOW the live top (arena+%ld), so it is inside live heap and this fault is NOT the vacated-ground trap\n", (long)(g_hp_top - g_hp_arena));
    if (g_gc_in) n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   THE FAULT HAPPENED INSIDE A COLLECTION -- read this as a defect in the collector or in the trap before reading it as a mutator stale pointer\n");
    n += snprintf(bf + n, sizeof bf - (size_t)n, "[ZGC-STALE]   a stale read means a ROOT THE COLLECTOR NEVER VISITED kept a pre-collection address; the instruction above is where it was used, not where it was stored. SCRIP_GC_TRAP=0 restores the old silent wrong answer.\n");
    if (n > 0) { ssize_t w = write(2, bf, (size_t)n); (void)w; }
    return 1;
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
    g_gc_poll_site = (const char *)__builtin_return_address(0);
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
    case DT_CPLX: {
        rt_hblk_t *h = gc_blk_of((const char *)d->p);
        if (!h) return;
        gc_mark_blk(h, 0);
        gc_slot_reg((void *)&d->p);
        return; }
    case DT_E: {
        if (!IS_PROCVAL_fn(*d)) return;
        rt_hblk_t *h = gc_blk_of(d->s);
        if (!h) return;
        gc_mark_blk(h, 0);
        if (d->s != (char *)(h + 1)) g_gc_interior++;
        gc_slot_reg((void *)&d->s);
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
static char *g_gc_emit_ceiling = (char *)0;
static int   g_gc_seg_main = 0;
_Static_assert(sizeof(void *) == 8, "A PARKED STACK IS A SEGMENT FROM ITS RECORDED STACK POINTER, NOT FROM ITS MAPPING (cto, 2026-09-23, row gc-heap-growth-...; law CEO-812 THE COLLECTOR GUESSES NOTHING): scrip_coswitch records rsp in park_sp before it posts the semaphore, every word of the mutator's lives at or above it, and the words below it are glibc's sem_wait frames -- the whole-mapping walk read those raw words as cells (the accident test_gate_gc_the_coexpression_roots_are_typed_and_the_parked_stacks_are_segments.sh names) and cost geddump 5.5 s per collection walking 8 MB per parked co-expression; SCRIP_GC_COEXPR_PLANT=3 restores the whole-mapping walk as the plant");
static char *gc_seg_parked_lo(scrip_coctx_t *c, char *lo) { return (scrip_co_gc_plant() != 3 && c->park_sp && c->park_sp > lo) ? c->park_sp : lo; }
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
    if (it->stage == 2) { it->stage = 3; if (it->floor < it->run_hi) { *lo = it->floor; *hi = it->run_hi; *pop = 1;
        g_gc_seg_main = (it->run_hi == gc_stack_top()) ? 1 : 0;
        if (g_gc_seg_main && g_gc_emit_ceiling && g_gc_emit_ceiling > it->floor && g_gc_emit_ceiling < it->run_hi) *hi = g_gc_emit_ceiling;
        return 1; } }
    return 0;
}
static long gc_stack_segments(char *floor)
{
    gc_seg_it_t it; char *lo, *hi; int pop; long words = 0;
    gc_seg_begin(&it, floor);
    while (gc_seg_next(&it, &lo, &hi, &pop)) { g_gc_rep_pop = pop; words += gc_visit_segment((const char *)lo, (const char *)hi); g_gc_rep_pop = 0; g_gc_seg_main = 0; }
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
        case DT_CPLX: return 1;
        case DT_P:    return 1;
        case DT_PLVAR: return 1;
        case DT_PLREF: return 1;
        case DT_E:    return IS_PROCVAL_fn(*d);
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
    if (d->v == DT_E && IS_PROCVAL_fn(*d)) { rt_hblk_t *eh = gc_blk_of(d->s); if (eh && d->s >= (char *)(eh + 1) && d->s < (char *)eh + eh->size) return 1; }
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN) return gc_dvec_ref_ok(d);
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) return 1;
    if (d->v == DT_BIG || d->v == DT_CPLX) { rt_hblk_t *nh = gc_blk_of((const char *)d->p); if (nh && (char *)d->p == (char *)(nh + 1)) return 1; }
    if (d->v == DT_P || d->v == DT_PLVAR || d->v == DT_PLREF) { rt_hblk_t *ph = gc_blk_of((const char *)d->p); if (ph) return 1; }
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
typedef struct gc_walk_t { long frames, roots, nomap, notab, i_descr, i_heap, i_badtag, i_phase, i_ptr, i_ptr_heap, i_raw, i_raw_heap, i_gap, h_words, h_cell_heap, h_raw_heap, s_words, s_cell_heap, s_raw_heap, a_words, a_heap; } gc_walk_t;
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
static int gc_tag_bears_ptr(uint8_t v) { return v == DT_S || v == DT_SNUL || v == DT_X || v == DT_A || v == DT_T || v == DT_N || v == DT_DATA || v == DT_P || v == DT_PLVAR || v == DT_PLREF || v == DT_E || v == DT_BIG || v == DT_CPLX; }
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
    if (d->v == DT_E && IS_PROCVAL_fn(*d)) { rt_hblk_t *eh = gc_blk_of(d->s); if (eh && d->s >= (char *)(eh + 1) && d->s < (char *)eh + eh->size) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (gc_dvec_ref_ok(d)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN && gc_block_exact((const char *)d->ptr, HB_DVEC)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_P || d->v == DT_PLVAR || d->v == DT_PLREF) { if (gc_blk_of((const char *)d->p)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_BIG) { rt_hblk_t *bh = gc_blk_of((const char *)d->p); if (bh && bh->type == HB_WSB && (char *)d->p == (char *)(bh + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_CPLX) { rt_hblk_t *ch = gc_blk_of((const char *)d->p); if (ch && ch->type == HB_WSB && (char *)d->p == (char *)(ch + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    return 0;
}
static int gc_nospine_cell(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_NO_SPINE_CELL"); v = (e && *e == '1') ? 1 : 0; } return v; }
static void gc_walk_words(const char *lo, const char *hi, int cls, const char *rlo, const char *graph, const char *base)
{
    gc_walk_t *g = &g_gw[g_gc_rep_pop];
    for (const char *p = lo; p + 8 <= hi; p += 8) { const char **w = (const char **)p; rt_hblk_t *h = gc_blk_of(*w);
        if (cls == 0) g->s_words++; else if (cls == 1) g->h_words++; else g->a_words++;
        if (cls != 2 && !(cls == 0 && gc_nospine_cell()) && p + 16 <= hi && gc_cell_visit((DESCR_t *)p)) { if (gc_blk_of(*(const char *const *)(p + 8))) { if (cls == 0) g->s_cell_heap++; else g->h_cell_heap++; } p += 8; if (cls == 0) g->s_words++; else g->h_words++; continue; }
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
        if (w < lo) w = (kind == GC_LAY_DESCR) ? w + (((lo - w) + 15) & ~15L) : lo;
        if (e > hi) e = hi;
        if (w >= e) continue;
        covered += e - w;
        if (blob) { g_gc_blob_entries++; if (kind == GC_LAY_DESCR) g_gc_blob_descr++; else if (kind == GC_LAY_PTR_GC) g_gc_blob_ptr++; else if (kind == GC_LAY_RAW) g_gc_blob_raw++; else g_gc_blob_code++; }
        if (kind == GC_LAY_DESCR) { for (const char *c = w; c + 16 <= e; c += 16) { DESCR_t *d = (DESCR_t *)c; g->i_descr++; if (!gc_tag_known(d->v)) { g->i_badtag++; if (gc_tag_known((uint8_t)(uintptr_t)d->p)) g->i_phase++; gc_walk_badtag(m->graph_name, (long)(c - anchor), d); } else { if (gc_tag_bears_ptr(d->v) && gc_blk_of((const char *)d->p)) g->i_heap++; rt_gc_visit_descr(d); } } continue; }
        for (const char *c = w; c + 8 <= e; c += 8) { const char **pw = (const char **)c; rt_hblk_t *h = gc_blk_of(*pw);
            if (kind == GC_LAY_PTR_GC) { g->i_ptr++; if (h) { g->i_ptr_heap++; rt_gc_visit_raw(pw); } continue; }
            g->i_raw++; if (h) { g->i_raw_heap++; gc_walk_site("RAW", m->graph_name, (long)(c - anchor), pw, h); if (blob && gc_type_moves(h->type)) { g_gc_blob_raw_in_heap++; if (gc_maps_on()) fprintf(stderr, "[GC-BLOB-RAW] graph=%s off=%ld kind=%u word=%p blk=%p type=%u\n", m->graph_name ? m->graph_name : "?", (long)(c - anchor), kind, (const void *)*pw, (const void *)h, (unsigned)h->type); } } } }
    if (span > covered) g->i_gap += (span - covered) / 8;
}
static int gc_ceiling_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_CEILING"); v = (e && *e) ? (*e != '0') : 1; } return v; }
static long g_gc_ceil_bytes;
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
        if (!m) { if (nf == 0) g->nomap++; if (above && g_gc_seg_main && gc_ceiling_on() && !gc_maps_on()) { g_gc_ceil_bytes += (long)(hi - p); return; } gc_walk_words(p, hi, above ? 2 : 0, lo, last, p); return; }
        nf++; g->frames++; if (nf == 1 && !above && !g_gc_top_graph) g_gc_top_graph = m->graph_name ? m->graph_name : "?";
        if (gc_maps_verbose()) fprintf(gc_maps_log(), "[GC-WALK-CELL] pop=%s cell=%p graph=%s frame_bytes=%u header_bytes=%u map_off=%lu flags=%u above=%d\n", g_gc_rep_popname[k], (const void *)c, m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, (unsigned long)m->map_off, m->flags, above);
        if (m->flags & GC_FRAME_MAP_BLOB) { const char *top = c + (long)m->frame_bytes + (long)m->header_bytes; if (top > hi) top = hi;
            gc_walk_words(p, c, above ? 2 : 0, lo, above ? last : m->graph_name, c); gc_walk_interior(c + (long)m->frame_bytes, m, lo, hi); p = top; }
        else { const char *base = c - (long)m->map_off, *slo = (base < p) ? p : base, *hlo = c + 16, *hhi = hlo + (long)m->header_bytes; if (hhi > hi) hhi = hi;
            { long before = g->s_raw_heap; gc_walk_words(p, slo, above ? 2 : 0, lo, above ? last : m->graph_name, base); if (nf == 1 && !above && g->s_raw_heap > before) gc_walk_dump(p, slo, m->graph_name); }
            if (m->flags & GC_FRAME_MAP_LAYOUT) gc_walk_interior(base, m, lo, hi); else g->notab++;
            gc_walk_words(hlo, hhi, 1, lo, m->graph_name, base); p = hhi; }
        last = m->graph_name;
        if (m->flags & GC_FRAME_MAP_ROOT) { g->roots++; { const char *q = p; const gc_frame_map_t *mn = (const gc_frame_map_t *)0; while (q + 16 <= hi && !gc_walk_cell(q, hi, &mn)) q += 8; above = mn ? 0 : 1; }
            if (above && gc_ceiling_on() && g_gc_seg_main && (!g_gc_emit_ceiling || (char *)p < g_gc_emit_ceiling)) { g_gc_ceil_bytes += (long)((g_gc_emit_ceiling ? g_gc_emit_ceiling : g_gc_stktop) - (char *)p); g_gc_emit_ceiling = (char *)p; } }
    }
}
static long g_gc_rtccb_heap;
static void gc_walk_print(void)
{
    if (!gc_maps_on()) { memset(g_gw, 0, sizeof g_gw); g_gw_lines = 0; g_gw_suppressed = 0; return; }
    if (g_gc_rtccb_heap) fprintf(stderr, "[GC-RTCCB] heap=%ld\n", g_gc_rtccb_heap);
    for (int k = 0; k < GC_REP_POPS; k++) { gc_walk_t *g = &g_gw[k]; if (!g->frames && !g->nomap && !g->s_words && !g->a_words) continue;
        fprintf(stderr, "[GC-WALK] pop=%-7s frames=%ld roots=%ld nomap=%ld notab=%ld i_descr=%ld i_heap=%ld i_badtag=%ld i_phase=%ld i_ptr=%ld i_ptr_heap=%ld i_raw=%ld i_raw_heap=%ld i_gap=%ld h_words=%ld h_cell_heap=%ld h_raw_heap=%ld s_words=%ld s_cell_heap=%ld s_raw_heap=%ld a_words=%ld a_heap=%ld divergence=%ld\n",
            g_gc_rep_popname[k], g->frames, g->roots, g->nomap, g->notab, g->i_descr, g->i_heap, g->i_badtag, g->i_phase, g->i_ptr, g->i_ptr_heap, g->i_raw, g->i_raw_heap, g->i_gap, g->h_words, g->h_cell_heap, g->h_raw_heap, g->s_words, g->s_cell_heap, g->s_raw_heap, g->a_words, g->a_heap, g->i_raw_heap + g->h_raw_heap + g->s_raw_heap + g->a_heap); }
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
    static int said = 0;
    if (v < 0) { const char *e = getenv("SCRIP_GC_PLANT_SHIFT"); v = (e && *e) ? atol(e) : 0; if (v < 0) v = 0; v &= ~15L; if (v > (1L << 20)) v = 1L << 20; }
    if (!v) return 0;
    if ((long)(g_hp_end - g_hp_top) > v) return v;
    if (!said) { said = 1;
        fprintf(stderr, "[GC-SHIFT] plant DECLINED at this collection: SCRIP_GC_PLANT_SHIFT=%ld was asked for and the arena has only %ld bytes of headroom, so THIS collection displaced nothing. The decline is per-collection and this line is printed ONCE per process, so it does not by itself mean the plant never ran -- LOOK FOR A LINE MATCHING '^\\[GC-SHIFT\\] plant:' (ANCHORED, and this decline deliberately does not spell that literal anywhere in itself: an unanchored grep for it matched THIS LINE and counted a DECLINE as an APPLICATION, which is exactly backwards -- measured by the coo 2026-09-21, applied=1 read off a shift=1048576 run that planted nothing). That line prints ONCE PER PROCESS with no telemetry asked for, so its ABSENCE means the plant never applied and every gate reading this run graded an UNPLANTED collection while passing. Measured by the cto 2026-09-21 on the cfo's flag, one pasrec run at SCRIP_HEAP_MB=1 and stress 3: a 4096-byte shift applies 495 times, 65536 applies 466, 262144 applies 373, and 1048576 applies ZERO with rc=0 and byte-identical stdout. A silent decline is indistinguishable from a plant that ran and proved the property, which is why it is no longer silent.\n",
                v, (long)(g_hp_end - g_hp_top)); }
    return 0;
}
static long gc_plant_shift_prefix(void)
{
    long prefix = 0; char *p = g_hp_arena;
    while (p < g_hp_top && ((rt_hblk_t *)p)->type == HB_FILL && ((rt_hblk_t *)p)->size) { prefix += ((rt_hblk_t *)p)->size; p += ((rt_hblk_t *)p)->size; }
    return prefix;
}
static int gc_plant_rtccb_slot(void)
{
    static int s = -2;
    if (s != -2) return s;
    { const char *e = getenv("SCRIP_GC_PLANT_RTCCB"); long v = (e && *e) ? atol(e) : -1;
      if (v < 0) { s = -1; return s; }
      if (v > 31 || v == RTCC_SLOT_R8 || v == RTCC_SLOT_R9) {
          fprintf(stderr, "[GC-RTCCB] plant DECLINED for the whole process: SCRIP_GC_PLANT_RTCCB=%ld names a slot this plant refuses. Slot %d is RTCC_SLOT_R8, the word rtcc.h declares as the ANCHOR, and it carries a STANDING population of heap references -- 806 of them over eight SNOBOL4 witnesses at arena 1 MB stress 3, measured by the cto 2026-09-20 and again 2026-09-21 -- so a plant there is INDISTINGUISHABLE FROM THE DEFECT IT EXISTS TO CATCH and could pass by accident on the very population it grades. Slot %d is RTCC_SLOT_R9, the GVA base every emitted program addresses its globals through, and overwriting it across a collection does not plant a defect, it breaks the program. A slot above 31 is outside the block. THE PLANT GOES IN SLOT 7 (r10) OR SLOT 8 (r11), which read a measured ZERO over all 52 witnesses in four languages at stress 3 and 5. This line does not spell the applied banner's literal, so a reader counting applications never counts a decline as one.\\n", v, RTCC_SLOT_R8, RTCC_SLOT_R9);
          s = -1; return s; }
      s = (int)v; }
    return s;
}
static int gc_reloc_forced(void)
{
    const char *e = getenv("SCRIP_GC_RELOC");
    return (e && *e && *e != '0') ? 1 : 0;
}
static void gc_displace_census(rt_hblk_t **liveo, uint64_t *livef, long li, long run, int forced)
{
    const char *e = getenv("SCRIP_GC_DISPLACE");
    long i, un = 0, dn = 0, up = 0, h0 = 0, h1 = 0, h2 = 0, h3 = 0, h4 = 0, mn = -1, mx = 0, sum = 0;
    if (!e || !*e || *e == '0') return;
    for (i = 0; i < li; i++) { long d = (long)((char *)livef[i] - (char *)liveo[i]), a = d < 0 ? -d : d;
        if (d == 0) un++; else if (d < 0) dn++; else up++;
        if (a == 0) h0++; else if (a < 4096) h1++; else if (a < 65536) h2++; else if (a < (1L << 20)) h3++; else h4++;
        if (a) { if (mn < 0 || a < mn) mn = a; if (a > mx) mx = a; sum += a; } }
    fprintf(stderr, "[GC-DISPLACE] run=%ld forced=%d live=%ld unmoved=%ld moved=%ld down=%ld up=%ld dmin=%ld dmax=%ld dmean=%ld hist zero=%ld lt4K=%ld lt64K=%ld lt1M=%ld ge1M=%ld\n",
        run, forced, li, un, dn + up, dn, up, mn < 0 ? 0L : mn, mx, (dn + up) ? sum / (dn + up) : 0L, h0, h1, h2, h3, h4);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifdef SCRIP_GC_AUDIT_B
static const rt_hblk_t *gc_audit_b_blk_of(const char *p) { return (const rt_hblk_t *)gc_blk_of(p); }
static const rt_hblk_t *gc_audit_b_blk_at(long i) { return (i >= 0 && i < g_gc_nblk && g_gc_idx) ? (const rt_hblk_t *)g_gc_idx[i] : (const rt_hblk_t *)0; }
static long gc_audit_b_birth_of(const char *blk, char *buf, long cap)
{
    long bk = gc_birth_find((char *)blk);
    if (bk < 0) return 0;
    { Dl_info d0, d1; gc_vac_t *v = &g_gc_vac[bk];
      const char *s0 = (dladdr(v->ra_site, &d0) && d0.dli_sname) ? d0.dli_sname : "?"; const char *s1 = (v->ra_from && dladdr(v->ra_from, &d1) && d1.dli_sname) ? d1.dli_sname : "?";
      return (long)snprintf(buf, (size_t)cap, "birth=#%ld by=%s from=%s born_at=arena+%ld", v->serial, s0, s1, (long)(v->at - g_hp_arena)); }
}
static long gc_audit_b_shim(char *floor)
{
    gc_audit_b_rgn_t rg[64]; gc_audit_b_skip_t sk[32]; gc_audit_b_t v; gc_seg_it_t it; char *lo, *hi; long n = 0, k = 0, i; int pop, saved = g_gc_seg_main;
    if (!g_hp_arena || !g_gc_idx) return -1;
    gc_static_segs_init();
    for (i = 0; i < g_gc_nseg && n < 64; i++) { rg[n].lo = g_gc_segs[i].lo; rg[n].hi = g_gc_segs[i].hi; rg[n].pop = "static"; rg[n].name = "writable-PT_LOAD"; n++; }
    gc_seg_begin(&it, floor);
    while (n < 64 && gc_seg_next(&it, &lo, &hi, &pop)) { rg[n].lo = lo; rg[n].hi = hi; rg[n].pop = "stack"; rg[n].name = g_gc_rep_popname[pop]; n++; }
    g_gc_seg_main = saved;
    sk[k].at = (const void *)&g_hp_fr;          sk[k].bytes = (long)sizeof g_hp_fr;          sk[k].name = "g_hp_fr";          k++;
    sk[k].at = (const void *)&g_hp_arena;       sk[k].bytes = (long)sizeof g_hp_arena;       sk[k].name = "g_hp_arena";       k++;
    sk[k].at = (const void *)&g_hp_gcline;      sk[k].bytes = (long)sizeof g_hp_gcline;      sk[k].name = "g_hp_gcline";      k++;
    sk[k].at = (const void *)&g_hp_win;         sk[k].bytes = (long)sizeof g_hp_win;         sk[k].name = "g_hp_win";         k++;
    sk[k].at = (const void *)&g_hp_wend;        sk[k].bytes = (long)sizeof g_hp_wend;        sk[k].name = "g_hp_wend";        k++;
    sk[k].at = (const void *)&g_hp_cap_end;     sk[k].bytes = (long)sizeof g_hp_cap_end;     sk[k].name = "g_hp_cap_end";     k++;
    sk[k].at = (const void *)&g_hp_qlo;         sk[k].bytes = (long)sizeof g_hp_qlo;         sk[k].name = "g_hp_qlo";         k++;
    sk[k].at = (const void *)&g_hp_qhi;         sk[k].bytes = (long)sizeof g_hp_qhi;         sk[k].name = "g_hp_qhi";         k++;
    sk[k].at = (const void *)&g_gc_pmap_top;    sk[k].bytes = (long)sizeof g_gc_pmap_top;    sk[k].name = "g_gc_pmap_top";    k++;
    sk[k].at = (const void *)&g_gc_mhead;       sk[k].bytes = (long)sizeof g_gc_mhead;       sk[k].name = "g_gc_mhead";       k++;
    sk[k].at = (const void *)g_gc_spine_rec;    sk[k].bytes = (long)sizeof g_gc_spine_rec;   sk[k].name = "g_gc_spine_rec";   k++;
    v.blk_of = gc_audit_b_blk_of; v.blk_at = gc_audit_b_blk_at; v.birth_of = gc_birth_on() ? gc_audit_b_birth_of : (long (*)(const char *, char *, long))0;
    v.alo = g_hp_arena; v.ahi = g_hp_top; v.run = g_gc_runs + 1; v.nblk = g_gc_nblk; v.rgn = rg; v.nrgn = n; v.skip = sk; v.nskip = k;
    return gc_audit_b_collect(&v);
}
#endif
static long gc_collect_ex(void)
{
    extern void kw_cset_gc_roots(void); extern void core_gc_roots(void); extern void dat_gc_roots(void); extern void gen_gc_roots(void); extern void pas_gc_roots(void); extern void pl_gc_roots(void); extern void rt_gc_root_args(void); extern void rt_gc_ws_roots(void); extern void eval_gc_roots(void); extern void lower_gc_roots(void); extern void bnd_gc_roots(void); extern void drv_gc_roots(void); extern int rt_scan_active(void);
    char anchor; long words = 0, interior = 0; long nlive = 0, nfill = 0, before_b, after_b, n_mk = 0, n_fw = 0, n_plant = 0; char *dest; rt_hblk_t **liveo; uint64_t *livef; long li = 0; long nforeign = 0;
    long w_cnt = 0, w_idx = 0, w_pmg = 0, w_fwd = 0, w_liv = 0, w_sld = 0, w_vfy = 0, w_cel = 0, w_raw = 0, w_mov = 0, w_unm = 0; int w_tel = getenv("SCRIP_ZETA_TELEM") ? 1 : 0;
    long n_rfz = 0; int reloc = gc_reloc_forced();
    int st_n = 0, st_slot[32]; uint64_t st_word[32], st_fwd[32]; rt_hblk_t *st_blk[32];
    int pl_slot = gc_plant_rtccb_slot(), pl_mark = 0; rt_hblk_t *pl_blk = (rt_hblk_t *)0; char *pl_addr = (char *)0; uint64_t pl_save = 0, pl_fwd = 0;
    double n_cnt = 0, n_idx = 0, n_mrk = 0, n_fwd = 0, n_liv = 0, n_sld = 0, n_vfy = 0, n_fix = 0, n_t0 = 0, n_all = w_tel ? gc_walk_ns() : 0;
    g_sxt_owner = (char *)0;
    if (g_gc_in || !g_hp_arena) return 0;
    g_gc_in = 1; before_b = (long)(g_hp_top - g_hp_arena);
    gc_quar_release(g_hp_cap_end);
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
    g_gc_mhead = (rt_hblk_t *)0; g_gc_spine_recn = 0; g_gc_top_graph = (const char *)0;
    g_gc_hn = 0; if (g_gc_hs) memset(g_gc_hs, 0, (size_t)g_gc_hcap * sizeof(void *));
    g_gc_nslot = 0; g_gc_interior = 0;
    if (pl_slot >= 0 && g_gc_nblk > 0) { pl_blk = g_gc_idx[g_gc_nblk - 1]; pl_addr = (char *)(pl_blk + 1); pl_save = rtccb[pl_slot]; rtccb[pl_slot] = (uint64_t)(uintptr_t)pl_addr; }
    { extern void pl_tr_gc_root_ball(const char *); for (long i = 0; i < g_gc_rrng_n; i++) if (!g_gc_rrng[i].hi) pl_tr_gc_root_ball(g_gc_rrng[i].lo); }
    for (long i = 0; i < g_gc_rrng_n; i++) { if (g_gc_rrng[i].hi) continue; { const char *top = *(const char * const *)g_gc_rrng[i].lo;
        for (char *e = (char *)g_gc_rrng[i].lo + 32; e + 32 <= top; e += 32) { const char **cell = (const char **)e; DESCR_t *old = (DESCR_t *)(e + 16); if (*cell) rt_gc_visit_raw(cell); rt_gc_visit_descr(old); } } }
    rt_gc_ws_roots();
    gc_coexpr_records(); words += gc_stack_segments(g_gc_seam_sp ? g_gc_seam_sp : &anchor);
    gc_root_cas();
    kw_cset_gc_roots(); core_gc_roots(); dat_gc_roots(); gen_gc_roots(); pas_gc_roots(); pl_gc_roots(); rt_gc_root_args(); eval_gc_roots(); lower_gc_roots(); bnd_gc_roots(); drv_gc_roots();
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
    { for (int ci = 0; ci < 32; ci++) { rt_hblk_t *hb = gc_blk_of((const char *)rtccb[ci]); if (!hb) continue; g_gc_rtccb_heap++;
        if (gc_maps_on()) fprintf(stderr, "[GC-WALK-RTCCB] slot=%d word=%p graph=%s line=%ld site=%p\n", ci, (const void *)rtccb[ci], g_gc_top_graph ? g_gc_top_graph : "?", g_line, (const void *)g_gc_poll_site);
        if (st_n < 32 && (w_tel || gc_maps_on())) { st_slot[st_n] = ci; st_word[st_n] = rtccb[ci]; st_blk[st_n] = hb; st_fwd[st_n] = 0; st_n++; } } }
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
#ifdef SCRIP_GC_AUDIT_B
    gc_audit_b_shim(g_gc_seam_sp ? g_gc_seam_sp : &anchor);
#endif
    gc_spine_lost_check();
    { static int cov = -1; if (cov < 0) { const char *e = getenv("SCRIP_GC_COVERAGE"); cov = (e && *e && *e != '0') ? 1 : 0; }
      if (cov) fprintf(stderr, "[GC-COV] ranges=%ld cas_scanned_bytes=%ld words_scanned=%ld interior_words=%ld ceiling_bytes_skipped=%ld ceiling=%p\n", g_gc_rrng_n, g_gc_cas_bytes, words, interior, g_gc_ceil_bytes, (void *)g_gc_emit_ceiling); }
    { long shift = gc_plant_shift_bytes(); long prefix = shift ? gc_plant_shift_prefix() : 0;
      if (shift) { long live_total = 0; for (long i = 0; i < g_gc_nblk; i++) if (g_gc_idx[i]->flags & HBF_MARK) live_total += (long)g_gc_idx[i]->size;
        if (g_hp_arena + prefix + shift + live_total > g_hp_end) { static int said_room = 0; if (!said_room) { said_room = 1;
            fprintf(stderr, "[GC-SHIFT] plant DECLINED at this collection: the fill prefix of %ld bytes left by earlier shifts plus the %ld-byte shift plus %ld live bytes would put the compacted live set past the committed window end at arena+%ld, and the plant moves nothing it cannot commit. Printed ONCE per process.\n", prefix, shift, live_total, (long)(g_hp_end - g_hp_arena)); }
          shift = 0; prefix = 0; } }
      g_gc_shift_prefix = prefix; dest = g_hp_arena + prefix + shift; g_gc_shift_now = shift; }
    { int fold = 1;
    if (fold) { gc_live_grow(0); liveo = g_gc_liveo; livef = g_gc_livef; }
    for (long i = 0; i < g_gc_nblk; i++) { rt_hblk_t *h = g_gc_idx[i];
        if (h->flags & HBF_MARK) { n_mk++; { int _pt = (gc_plant_pin_type() < 0 || (long)h->type == gc_plant_pin_type()) ? 1 : 0; if (_pt) n_plant++; if (n_plant == gc_plant_pin_skip() && _pt) { h->fwd = 0; dest += h->size; }
            else { if (reloc && (char *)dest == (char *)h) { if (dest + 2 * (long)sizeof(rt_hblk_t) + h->size <= g_hp_end) dest += 2 * (long)sizeof(rt_hblk_t); else n_rfz++; } h->fwd = (uint64_t)dest; dest += h->size; nlive++; } } }
        else h->fwd = 0;
        if (h->fwd) n_fw++;
        if (fold && h->fwd) { if (li >= g_gc_lcap) { gc_live_grow(li); liveo = g_gc_liveo; livef = g_gc_livef; } liveo[li] = h; livef[li] = h->fwd; li++; } }
    if (w_tel) { w_fwd = g_gc_nblk; n_fwd = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    gc_displace_census(liveo, livef, li, g_gc_runs + 1, reloc);
    if (reloc && n_rfz) fprintf(stderr, "[GC-RELOC] REFUSED to displace %ld live block(s): no room for a minimum legal block (32 bytes) of gap, so they kept their address and THIS COLLECTION IS NOT A FORCED-RELOCATION MEASUREMENT\n", n_rfz);
    gc_vac_record(dest); }
    if (pl_blk) { pl_mark = (pl_blk->flags & HBF_MARK) ? 1 : 0; pl_fwd = pl_blk->fwd; }
    for (int z = 0; z < st_n; z++) st_fwd[z] = st_blk[z]->fwd;
    if (n_mk != n_fw) fprintf(stderr, "[ZGC-PIN] VIOLATION marked=%ld forwarded=%ld skipped=%ld -- a marked block was not given a forwarding address, so it keeps its address while the heap slides around it: that is PINNING under another name, and no pinning mechanism returns in any form (Lon 2026-09-17, CEO-831)\n", n_mk, n_fw, n_mk - n_fw);
    for (long i = 0; i < g_gc_nslot; i++) { gc_slot_t *sl = &g_gc_slots[i]; const char **loc = sl->hloc ? (const char **)((char *)(sl->hloc + 1) + sl->off) : (const char **)sl->off;
        rt_hblk_t *h = gc_blk_of(*loc); if (h && h->fwd && h->fwd != (uint64_t)h) *loc = (const char *)((rt_hblk_t *)h->fwd + 1) + (*loc - (const char *)(h + 1)); }
    if (w_tel) { w_cel = g_gc_nslot; w_raw = 0; n_fix = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (w_tel) n_t0 = gc_walk_ns();
    dest = g_hp_arena;
    if (g_gc_shift_now) {
        if (li > 0) gc_quar_release((char *)livef[li - 1] + liveo[li - 1]->size);
        for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] == (char *)h) w_unm++; if ((char *)livef[i] < (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        for (long i = li - 1; i >= 0; i--) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] > (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        { rt_hblk_t *fl = (rt_hblk_t *)(g_hp_arena + g_gc_shift_prefix); fl->fwd = 0; fl->size = (uint32_t)g_gc_shift_now; fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
          for (char *q = g_hp_arena; q < g_hp_arena + g_gc_shift_prefix; q += ((rt_hblk_t *)q)->size) nfill++; }
        dest = li ? (char *)livef[li - 1] + ((rt_hblk_t *)livef[li - 1])->size : g_hp_arena + g_gc_shift_prefix + g_gc_shift_now;
        { static int said_applied = 0;
          if (w_tel || gc_maps_on() || !said_applied) { said_applied = 1;
              fprintf(stderr, "[GC-SHIFT] plant: every live block forwarded %ld bytes up, over %ld bytes of kept fill at the arena start -- a stale copy of any heap address is wrong after every collection. THIS LINE IS THE ONLY PROOF THE PLANT APPLIED, so it prints ONCE PER PROCESS with nothing asked for, and once per collection under SCRIP_ZETA_TELEM or SCRIP_GC_MAPS. It was telemetry-gated until 2026-09-21, so a reader following the decline line's own advice without telemetry counted ZERO applications on EVERY run and could conclude the plant never fires when it fires at every collection: measured by the cto on the coo's flag, hb_coexpr_create.icn at SCRIP_HEAP_MB=1 stress 3 shift 4096 applies 199 times and showed 0 of them.\n", g_gc_shift_now, g_gc_shift_prefix + g_gc_shift_now); } }
    } else if (reloc) {
        for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] == (char *)h) w_unm++; if ((char *)livef[i] < (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        for (long i = li - 1; i >= 0; i--) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size; if ((char *)livef[i] > (char *)h) { memmove((void *)livef[i], (void *)h, (size_t)sz); if (w_tel) w_mov += (long)sz; } }
        { char *q = g_hp_arena;
          for (long i = 0; i < li; i++) { char *nb = (char *)livef[i];
              if (nb > q) { rt_hblk_t *fl = (rt_hblk_t *)q; fl->fwd = 0; fl->size = (uint32_t)(nb - q); fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
                  if ((long)fl->size > (long)(g_hp_wend - g_hp_win)) { g_hp_win = q; g_hp_wend = nb; } }
              q = nb + ((rt_hblk_t *)nb)->size; }
          dest = q; }
    } else
    for (long i = 0; i < li; i++) { rt_hblk_t *h = liveo[i]; uint32_t sz = h->size;
        if ((char *)livef[i] == (char *)h) { w_unm++; if (dest < (char *)h) { rt_hblk_t *fl = (rt_hblk_t *)dest; fl->fwd = 0; fl->size = (uint32_t)((char *)h - dest); fl->type = HB_FILL; fl->flags = HBF_TTL; nfill++;
            if ((long)fl->size > (long)(g_hp_wend - g_hp_win)) { g_hp_win = dest; g_hp_wend = (char *)h; } } dest = (char *)h + sz; }
        else { memmove((void *)livef[i], (void *)h, (size_t)sz); dest = (char *)livef[i] + sz; if (w_tel) w_mov += (long)sz; } }
    g_hp_top = dest; g_hp_blocks = nlive + nfill; g_hp_live = (long)(dest - g_hp_arena); rt_gcheap_line_reset();
    for (long i = 0; i < li; i++) { rt_hblk_t *nh = (rt_hblk_t *)livef[i]; nh->fwd = 0; nh->flags = (uint16_t)((nh->flags | HBF_TTL) & ~HBF_MARK); }
    after_b = (long)(g_hp_top - g_hp_arena);
    if (st_n) { static int said_stale = 0; long mv = 0, un = 0, sw = 0;
        for (int z = 0; z < st_n; z++) { int moved = (st_fwd[z] && st_fwd[z] != (uint64_t)(uintptr_t)st_blk[z]) ? 1 : 0, kept = (rtccb[st_slot[z]] == st_word[z]) ? 1 : 0;
            if (moved) mv++; if (moved && kept) un++; if (!st_fwd[z] && kept) sw++;
            if (gc_maps_on()) fprintf(stderr, "[GC-RTCCB-STALE] slot=%d word=%p block=type%d/%ldB forwarded=%d moved=%d unrepaired=%d swept=%d graph=%s line=%ld site=%p\n",
                st_slot[z], (const void *)(uintptr_t)st_word[z], (int)st_blk[z]->type, (long)st_blk[z]->size, st_fwd[z] ? 1 : 0, moved, (moved && kept) ? 1 : 0, (!st_fwd[z] && kept) ? 1 : 0, g_gc_top_graph ? g_gc_top_graph : "?", g_line, (const void *)g_gc_poll_site); }
        if ((un || sw) && (w_tel || gc_maps_on() || !said_stale)) { said_stale = 1;
            fprintf(stderr, "[GC-RTCCB-STALE] SUMMARY collection=%ld slots-holding-a-heap-pointer=%d moved=%ld UNREPAIRED=%ld SWEPT=%ld. This is the STANDING population and not a plant: these are words the emitter itself spilled into the caller-saved block before a safe point (x86_asm.h 386-389), and the block is registered as a GC root by rtcc_init.c while the collector's only walk over the range table skips exactly that shape, so nothing visits them. UNREPAIRED means the collector FORWARDED the block the word names and left the word holding the pre-move address, which the emitted reload after the poll then restores into a register; SWEPT means the block was not forwarded at all and the word names ground the arena will re-issue. A pointer in an unvisited slot is necessary but not sufficient for a lost value -- the same block may be rooted elsewhere, which is how these witnesses still answer their oracles -- but UNREPAIRED and SWEPT are the two ways it stops being merely necessary, and they are counted here rather than argued.\n",
                g_gc_runs + 1, st_n, mv, un, sw); } }
    if (pl_blk) { static int said_rtccb = 0; uint64_t now = rtccb[pl_slot]; char *nw = pl_fwd ? (char *)((rt_hblk_t *)(uintptr_t)pl_fwd + 1) : (char *)0;
        int moved = (pl_fwd && pl_fwd != (uint64_t)(uintptr_t)pl_blk) ? 1 : 0, kept = (now == (uint64_t)(uintptr_t)pl_addr) ? 1 : 0;
        if (w_tel || gc_maps_on() || !said_rtccb) { said_rtccb = 1;
            fprintf(stderr, "[GC-RTCCB] plant: slot=%d word=%p block=type%d/%ldB live=%d moved=%d forwarded-payload=%p slot-after-collection=%p unrepaired=%d swept=%d. THIS LINE IS THE ONLY PROOF THE PLANT APPLIED and it prints ONCE PER PROCESS with nothing asked for, then once per collection under SCRIP_ZETA_TELEM or SCRIP_GC_MAPS. WHAT IT PLANTS: the payload address of the most recently carved block -- which is what a call result live across a safe point IS -- written into a caller-saved spill slot at the top of a collection and RESTORED BYTE FOR BYTE at the end of it, so the program answers identically with the plant on and off. WHAT IT PROVES: the detector is looking (the run prints a GC-WALK-RTCCB line naming this slot under SCRIP_GC_MAPS, so a green reading of the invariant is a measurement and not a silence), and a line whose unrepaired and moved fields are BOTH SET is the STALENESS half measured rather than argued (this sentence deliberately spells neither field with its value, because a decline or an explanation that spells the literal a reader is told to count is counted AS one, which is the defect cured at CTO-113 in this same file) -- no root walk visits this block, so nothing rewrites the slot when the collector forwards the value it holds, and the emitted reload after the poll restores a word that names ground the collector has already given away. WHAT IT IS NOT: this is not the emitter road. The real road is the spill at x86_asm.h 386-398, and its standing population lives in slot %d.\\n",
                pl_slot, (const void *)pl_addr, (int)pl_blk->type, (long)pl_blk->size, pl_mark, moved, (const void *)nw, (const void *)(uintptr_t)now, (moved && kept) ? 1 : 0, (!pl_mark && kept) ? 1 : 0, RTCC_SLOT_R8); }
        rtccb[pl_slot] = pl_save; }
    { static int psn = -1; if (psn < 0) { const char *e = getenv("SCRIP_GC_POISON"); psn = (e && *e) ? (*e != '0') : 1; } if (psn) { char *ot = g_hp_arena + before_b; long pb = ot > g_hp_top ? (long)(ot - g_hp_top) : 0; if (pb > 0) memset(g_hp_top, 0xDB, (size_t)pb); if (w_tel) fprintf(stderr, "[ZGC-POISON] vacated=%ldB filled=0xDB top=%p oldtop=%p\n", pb, (void *)g_hp_top, (void *)ot); } }
    if (w_tel) { w_sld = li; n_sld = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    { extern void rt_nv_memo_invalidate(void); rt_nv_memo_invalidate(); }
    rt_gcheap_verify();
    if (w_tel) { w_vfy = nlive + nfill; n_vfy = gc_walk_ns() - n_t0; }
    g_gc_runs++;
    if (w_tel) fprintf(stderr, "[ZGC-WALK] arm=%s nblk=%ld | count=%ld/%.0fus index=%ld/%.0fus pmap-gran=%ld fwd=%ld/%.0fus live=%ld/%.0fus | mark=%.0fus fixup=%ld+%ld/%.0fus slide=%ld/%.0fus moved=%ldB unmoved=%ld verify=%ld/%.0fus | walk-floor=%ld titles %.0fus of %.0fus total\n",
        "FOLD", g_gc_nblk, w_cnt, n_cnt / 1e3, w_idx, n_idx / 1e3, w_pmg, w_fwd, n_fwd / 1e3, w_liv, n_liv / 1e3, n_mrk / 1e3, w_cel, w_raw, n_fix / 1e3, w_sld, n_sld / 1e3, w_mov, w_unm, w_vfy, n_vfy / 1e3,
        w_cnt + w_idx + w_fwd + w_liv + w_vfy, (n_cnt + n_idx + n_fwd + n_liv + n_vfy) / 1e3, (gc_walk_ns() - n_all) / 1e3);
    if (g_gc_dvec_elems && (w_tel || gc_maps_on())) fprintf(stderr, "[GC-DVEC] elems=%ld non_dvec=%ld\n", g_gc_dvec_elems, g_gc_dvec_nondvec);
    if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZGC] regeneration #%ld (%s): blocks %ld->%ld (fill %ld) bytes %ld->%ld reclaimed %ld win=%ld slots=%ld interior=%ld wl_depth_max=%ld marked=%ld forwarded=%ld\n", g_gc_runs, "E", g_gc_nblk, nlive, nfill, before_b, after_b, before_b - after_b, (long)(g_hp_wend - g_hp_win), g_gc_nslot, g_gc_interior, g_gc_wlmax, n_mk, n_fw);
    g_hp_gcline = g_hp_top + gc_line_span((long)((g_hp_end - g_hp_top) >> 1));
    g_hp_fr.line = gc_line_paced() ? g_hp_gcline : g_hp_end;
    gc_quar_arm(g_hp_top);
    if (w_tel) fprintf(stderr, "[ZGC-TRAP] quarantine arena+%ld..arena+%ld (%ld pages PROT_NONE) ledger=%ld entries over=%ld arms=%ld\n", g_hp_qlo ? (long)(g_hp_qlo - g_hp_arena) : -1L, g_hp_qhi ? (long)(g_hp_qhi - g_hp_arena) : -1L, g_hp_qlo ? (long)((g_hp_qhi - g_hp_qlo) / (long)gc_pg()) : 0L, g_gc_vacn, g_gc_vacover, g_hp_qarm);
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
    if (g_gc_in) return;
    if (!g_gc_pending && !(g_hp_gcline && g_hp_top > g_hp_gcline)) return;
    g_gc_polls++;
    rt_gc_point_arr_c((DESCR_t *)0, 0, (const char **)0, (char *)__builtin_frame_address(0) + 16);
}
_Static_assert(sizeof(void *) == 8, "rt_gc_poll's floor is its caller's stack pointer: the frame pointer plus the saved rbp and the return address, 16 bytes on x86-64 -- the walk steps 8-byte words from this floor and never aligns it, so a floor taken from a char local (an odd address) misreads every cell above it (cto 2026-09-23, user_function_opsyn_8 printed empty at the association tap's spilled-pair poll)");
long rt_gc_polls_count(void) { return g_gc_polls; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_poll_slow(void);
void rt_gc_poll_asm(void);
__asm__(
".text\n"
".globl rt_gc_poll_slow\n"
".type rt_gc_poll_slow,@function\n"
"rt_gc_poll_slow:\n"
"  pushq %rax\n"
"  pushq %rcx\n"
"  pushq %rdx\n"
"  pushq %rsi\n"
"  pushq %rdi\n"
"  pushq %r8\n"
"  pushq %r9\n"
"  pushq %r10\n"
"  pushq %r11\n"
"  subq  $16, %rsp\n"
"  movq  %r13, (%rsp)\n"
"  incq g_gc_polls(%rip)\n"
"  movq 88(%rsp), %rax\n"
"  movq %rax, g_gc_poll_site(%rip)\n"
"  xorl %edi, %edi\n"
"  xorl %esi, %esi\n"
"  movq %rsp, %rdx\n"
"  leaq 88(%rsp), %rcx\n"
"  movl $1, %r8d\n"
"  call gc_point_arr_body\n"
"  movq (%rsp), %r13\n"
"  addq  $16, %rsp\n"
"  popq %r11\n"
"  popq %r10\n"
"  popq %r9\n"
"  popq %r8\n"
"  popq %rdi\n"
"  popq %rsi\n"
"  popq %rdx\n"
"  popq %rcx\n"
"  popq %rax\n"
"  ret\n"
".size rt_gc_poll_slow,.-rt_gc_poll_slow\n"
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__asm__(
".text\n"
".globl rt_gc_poll_asm\n"
".type rt_gc_poll_asm,@function\n"
"rt_gc_poll_asm:\n"
"  cmpl $0, g_gc_in(%rip)\n"
"  jne  9f\n"
"  pushq %r11\n"
"  movq g_gc_pending@GOTPCREL(%rip), %r11\n"
"  cmpl $0, (%r11)\n"
"  jne  2f\n"
"  movq g_hp_gcline(%rip), %r11\n"
"  testq %r11, %r11\n"
"  je   1f\n"
"  pushq %r10\n"
"  movq g_hp_fr@GOTPCREL(%rip), %r10\n"
"  movq (%r10), %r10\n"
"  cmpq %r11, %r10\n"
"  popq %r10\n"
"  jbe  1f\n"
"2:\n"
"  popq %r11\n"
"  jmp  rt_gc_poll_slow@PLT\n"
"1:\n"
"  popq %r11\n"
"9:\n"
"  ret\n"
".size rt_gc_poll_asm,.-rt_gc_poll_asm\n"
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_free(void) { if (!g_hp_arena) rt_gcheap_init(); return (long)(g_hp_end - g_hp_top); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_cb_open(void) { return rt_gc_runs_count(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_cb_close(long mark, const char *file, int line, void *lo, void *hi)
{
    long n = 0;
    if (rt_gc_runs_count() == mark) return 0;
    if (!g_hp_arena || !lo || !hi || (char *)hi <= (char *)lo) return 0;
    { char **w = (char **)((((uintptr_t)lo) + 7u) & ~(uintptr_t)7u);
      for (; (char *)(w + 1) <= (char *)hi; w++) { char *v = *w; if (v > g_hp_arena && v < g_hp_top) n++; } }
    if (n) {
        static int loud = -1; if (loud < 0) { const char *e = getenv("SCRIP_GC_CB_REPORT"); loud = (e && *e != '0') ? 1 : 0; }
        if (loud) fprintf(stderr, "[ZGC-CB] %s:%d %ld raw arena word(s) live in an UNMAPPED C FRAME across a callback during which the collector ran %ld time(s). Lon 2026-09-21: a C function creates residue on the hardware stack; no compile-time frame map describes this frame, so these words were not updated and the collector could not have updated them without guessing (ARCH-GC section 7, section 9(ii) as reversed at CEO-1090).\n", file ? file : "?", line, n, rt_gc_runs_count() - mark);
        { static int fatal = -1; if (fatal < 0) { const char *e = getenv("SCRIP_GC_CB_FATAL"); fatal = (e && *e != '0') ? 1 : 0; }
          if (fatal) { fprintf(stderr, "[ZGC-CB] SCRIP_GC_CB_FATAL is set: aborting on the first Rule 4 violation.\n"); abort(); } }
    }
    return n;
}
long rt_gc_runs_count(void) { return g_gc_runs; }
