#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <link.h>
#include <time.h>
#include <dlfcn.h>
#include <unwind.h>
#include "rt_slab.h"
#include "rt_arena.h"
#include "gc_heap.h"
#define GC_HEAP_MB 1
#define GC_HEAP_KB 1024
#define GC_RESERVE_FLOOR_MB 512
#define GC_HEAP_CAP_KB 131072
_Static_assert(GC_HEAP_KB == 1024 && GC_HEAP_CAP_KB == 131072,
    "THE DEFAULTS ARE SPITBOL'S (Lon 2026-09-25, in-chat to the ceo, verbatim: \"Let's set our default stack size and heap size for SCRIP to be the same as SPITBOL.\"; ceo CEO-1261): sbl prints its "
    "option defaults as -d128m -i1m -m16m -s4m: the initial window is -i1m and the hard cap -d128m; the window is also the least each growth commits (rt_gcheap_grow), which is -i's second meaning; t"
    "he 128 KB arena of CEO-1095 is the GC-testing instrument (make test-arena, SCRIP_HEAP_KB_TINY) and no longer the shipped default");
#define GC_HEAP_KB_FLOOR 64
#define GC_ERR_RESERVE 4096u
_Static_assert(GC_ERR_RESERVE == 4096u,
    "THE ERROR RESERVE IS SPITBOL'S e_srs, HELD INSIDE THE CAP (ceo CEO-1704): sbl.min keeps e_srs = 30 words back at the end of storage and its alloc (alc3b) adds them ONCE before error 204, "
    "so a SETEXIT handler can still allocate; SCRIP's heap commits by the 4 KB page, so the reserve is the cap's last page, rt_gcheap_grow never commits it, the exhausted allocator commits it once "
    "(released is g_hp_end == g_hp_cap_end) and the heap never passes its cap");
#define GC_FREE_PCT 15
_Static_assert(GC_FREE_PCT == 15,
    "A WINDOW IS SPENT BEFORE IT IS COLLECTED, AS SPITBOL SPENDS ITS -i (ceo 2026-09-25, CEO-1264; Lon 2026-09-25 10:06, in-chat to the ceo: \"continue getting all demos and benchmarks running faste"
    "r than SPITBOL\"): SPITBOL's alloc (sbl.min aloc1..aloc4) collects only when an allocation does not fit its dynamic area and then, when less than e_fsp = 15 percent of the area is free after th"
    "e collection, asks sysmm for another -i increment; SCRIP collected at HALF its free window, so under the demo grid's -d512m -i64m treebank collected once (110 ms: 814,366 blocks, 193,546 live, "
    "mark 87 ms) where SPITBOL collected zero times. The line now leaves one sixteenth of the free window as burst room between polls (the allocator never collects, so a burst past the line is commi"
    "tted, never refused), and a collection that leaves less than GC_FREE_PCT percent of the committed window free grows it at once (rt_gcheap_grow(0): the larger of the committed size and -i, up to"
    " the hard cap) instead of collecting again one sixteenth later");
#include "descr.h"
#include "pin_va.h"
#include "gc_frame_map.h"
#include "rt_pl_trail.h"
#include "gc_audit_b.h"
#include "../rtx/rtcc.h"
_Static_assert(sizeof(rt_hblk_t) == 16, "rt_hblk_t must be one 16-byte title unit");
typedef struct rt_hp_fr_t {
    char *top;
    char *end;
    long blocks;
    int armed;
    int _pad;
    char *virgin;
    int zfull;
    int _pad2;
    char *line;
    long alloc_total;
    long alloc_str;
    uint64_t *live, *start_abs, *vis, *start, *ssum, *prefix;
    long ngran;
} rt_hp_fr_t;
rt_hp_fr_t g_hp_fr = { (char *)0, (char *)0, 0, 0, 0, (char *)0, -1, 0, (char *)0, 0, 0, (uint64_t *)0, (uint64_t *)0, (uint64_t *)0, (uint64_t *)0, (uint64_t *)0, (uint64_t *)0, 0 };
_Static_assert(__builtin_offsetof(rt_hp_fr_t, live) == 72 && __builtin_offsetof(rt_hp_fr_t, start_abs) == 80 && __builtin_offsetof(rt_hp_fr_t, vis) == 88 &&
    __builtin_offsetof(rt_hp_fr_t, start) == 96 && __builtin_offsetof(rt_hp_fr_t, ssum) == 104 && __builtin_offsetof(rt_hp_fr_t, prefix) == 112 && __builtin_offsetof(rt_hp_fr_t, ngran) == 120,
    "the collector's granule bitmaps ride the frontier record at these offsets (ARCH-GC section 15; rtx_alloc.s sets the START bit through start_abs at 80)");
_Static_assert(sizeof(rt_hp_fr_t) == 128,
    "RTX-2 extends the PL-SINK-3 cell; 0/8/16/24 stay put; the pacing line rides at 48; the two allocation counters ride at 56 and 64 so emitted code reaches them on the base it already holds; the c"
    "ollector's bitmaps follow at 72..120");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, top) == 0, "PL-SINK-3 bakes g_hp_fr.top @0");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, end) == 8, "PL-SINK-3 bakes g_hp_fr.end @8");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, blocks) == 16, "PL-SINK-3 bakes g_hp_fr.blocks @16");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, armed) == 24, "PL-SINK-3 bakes g_hp_fr.armed @24");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, virgin) == 32, "RTX-2 bakes g_hp_fr.virgin @32");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, zfull) == 40, "RTX-2 bakes g_hp_fr.zfull @40");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, line) == 48, "rtx_alloc.s compares the inline carve against g_hp_fr.line @48 (the pacing line, or the arena end when pacing is off)");
_Static_assert(__builtin_offsetof(rt_hp_fr_t, alloc_total) == 56,
    "the &allocated total rides at 56: a hidden global cannot be named by emitted code and an exported one cannot be reached RIP-direct from rtx_alloc.s (measured: the shared object refuses to link)")
    ;
_Static_assert(__builtin_offsetof(rt_hp_fr_t, alloc_str) == 64, "the &allocated string total rides at 64");
#define g_hp_top (g_hp_fr.top)
#define g_hp_end (g_hp_fr.end)
#define g_hp_blocks (g_hp_fr.blocks)
#define g_hp_virgin (g_hp_fr.virgin)
static char *g_hp_arena = (char *)0;
static char *g_hp_gcline = (char *)0;
static char *g_hp_win = (char *)0;
static char *g_hp_wend = (char *)0;
static char *g_hp_cap_end = (char *)0;
static size_t g_hp_chunk = 0;
static long g_hp_sw_win_kb = 0, g_hp_sw_cap_kb = 0;
void rt_heap_size_set(long win_kb, long cap_kb) { if (win_kb > 0) g_hp_sw_win_kb = win_kb; if (cap_kb > 0) g_hp_sw_cap_kb = cap_kb; }
#if RT_DIAG
static long g_hp_capped = 0;
static long g_hp_grown = 0;
static long g_hp_grows = 0;
#endif
static long g_hp_live = 0;
static int g_hp_report_reg = 0;
static void gc_static_segs_init(void);
#if RT_DIAG
static void gc_birth_record(char *at, uint64_t total, uint16_t type, void *ra_site, void *ra_from, void **fp_from);
#endif
static char *gc_stack_top(void);
static void gc_knobs_hoist(void);
static int gc_diag_on(void);
static int gc_chain_check_on(void);
#if RT_DIAG
static int gc_trap_on(void);
static int gc_verify_on(void);
static int gc_telem_on(void);
static int gc_maps_on(void);
static int gc_walk_census_on(void);
#endif
#if RT_DIAG
typedef struct gc_vac_t { char *at; char *fwd; uint32_t size; uint32_t gen; uint16_t type; uint16_t pad; long serial; void *ra_site; void *ra_from; } gc_vac_t;
static gc_vac_t *g_gc_vac = (gc_vac_t *)0;
static long g_gc_vacn = 0, g_gc_vaccap = 0, g_gc_vacover = 0, g_gc_vacgen = 0;
static gc_vac_t *gc_vac_ledger(void);
static int gc_birth_on(void);
static char *g_hp_qlo = (char *)0;
static char *g_hp_flo = (char *)0;
static char *g_hp_fhi = (char *)0;
static char *g_gc_flip_to = (char *)0;
static long g_gc_flips = 0, g_gc_flip_declined = 0, g_gc_flip_live = 0;
static int gc_plant_flip(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_PLANT_FLIP"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
static char *g_hp_qhi = (char *)0;
static long g_hp_qgen = 0, g_hp_qarm = 0, g_hp_qbytes = 0;
static void gc_quar_release(char *need_end);
static void gc_quar_arm(char *lo);
static void gc_fr_line_sync(void) { g_hp_fr.line = (g_hp_qlo && g_hp_qlo < g_hp_gcline) ? g_hp_qlo : g_hp_gcline; }
#else
static void gc_fr_line_sync(void) { g_hp_fr.line = g_hp_gcline; }
#endif
int g_gc_pending;
#if RT_DIAG
static long g_gc_polls = 0;
static const char *g_gc_top_graph = (const char *)0;
static const char *g_gc_poll_site = (const char *)0;
#endif
extern long g_line;
static int g_gc_in;
__attribute__((visibility("hidden"))) rt_sxt_fr_t g_sxt_fr = { (char *)0, 0, 0, -1 };
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, owner) == 0, "rtx_str.s bakes g_sxt_fr.owner @0");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, len) == 8, "rtx_str.s bakes g_sxt_fr.len @8");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, gva_n) == 16, "rtx_str.s bakes g_sxt_fr.gva_n @16");
_Static_assert(__builtin_offsetof(rt_sxt_fr_t, off) == 20, "rtx_str.s bakes g_sxt_fr.off @20");
rt_sxt_fr_t * const rt_sxt_fr_p = &g_sxt_fr;
#define g_sxt_len (g_sxt_fr.len)
#define g_sxt_gva_n (g_sxt_fr.gva_n)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_line_mb(void) { static long line_mb = -1; if (line_mb < 0) { const char *e = getenv("SCRIP_GC_LINE_MB"); line_mb = e ? atol(e) : 128; if (line_mb < 0) line_mb = 0; } return line_mb; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_line_span(long room) { long line_mb = gc_line_mb(); if (line_mb == 0) return room; { long span = line_mb << 20; return span < room ? span : room; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_gva_count(int n) { g_sxt_gva_n = n; }
int rt_sxt_gva_count_get(void) { return g_sxt_gva_n; }
void rt_sxt_break(const char *s) { rt_sxt_break_fast(s); }
void rt_sxt_frames_present(void) { g_sxt_fr.off = 1; g_sxt_owner = (char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sxt_note(char *s, long len) {
    g_sxt_owner = (char *)0;
    if (!s || !g_hp_arena || len < 0) return;
    if (s < g_hp_arena + sizeof(rt_hblk_t) || s >= g_hp_top) return;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if ((h->flags & HBF_TTL) && h->type == (uint16_t)DT_S && (char *)h + h->size == g_hp_top) { g_sxt_owner = s; g_sxt_len = len; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_sxt_match(const char *s) { if (g_sxt_fr.off < 0) g_sxt_fr.off = 0; if (g_sxt_fr.off) return -1; return (s && s == g_sxt_owner) ? g_sxt_len : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_sxt_extend(char *s, long al, long bl) {
    if (!s || s != g_sxt_owner || al != g_sxt_len || al < 0 || bl < 0) return (char *)0;
    rt_hblk_t *h = ((rt_hblk_t *)s) - 1;
    if (!(h->flags & HBF_TTL) || h->type != (uint16_t)DT_S || (char *)h + h->size != g_hp_top) { g_sxt_owner = (char *)0; return (char *)0; }
    DESCR_t *gv = (DESCR_t *)RT_GVA_VA;
    int refs = 0;
    {
        int k = 0, gn = g_sxt_gva_n;
        for (; k + 4 <= gn; k += 4) {
            if (__builtin_expect(gv[k].s == s, 0) && gv[k].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
            if (__builtin_expect(gv[k + 1].s == s, 0) && gv[k + 1].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
            if (__builtin_expect(gv[k + 2].s == s, 0) && gv[k + 2].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
            if (__builtin_expect(gv[k + 3].s == s, 0) && gv[k + 3].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
        }
        for (; k < gn; k++) if (gv[k].s == s && gv[k].v == DT_S && ++refs > 1) { g_sxt_owner = (char *)0; return (char *)0; }
    }
    uint64_t want = sizeof(rt_hblk_t) + (((uint64_t)(al + bl + 1) + 15u) & ~15ull);
    if (want > h->size) {
        uint64_t d = want - h->size;
        if (g_hp_top + d > g_hp_end) { g_sxt_owner = (char *)0; return (char *)0; }
#if RT_DIAG
        gc_quar_release(g_hp_top + d);
#endif
        h->size = (uint32_t)want;
        g_hp_top += d;
        g_hp_fr.alloc_total += (long)d;
        g_hp_fr.alloc_str += (long)d;
    }
    { uint64_t pay = want - sizeof(rt_hblk_t); uint64_t used = (uint64_t)(al + bl + 1); if (pay > used) memset(s + used, 0, (size_t)(pay - used)); }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
long rt_gcheap_verify(void) {
    long n = 0, bits = 0;
    char *p = g_hp_arena;
    size_t topg = (size_t)(g_hp_top - g_hp_arena) >> 4, nw = (topg + 63) >> 6;
    while (p && p < g_hp_top) {
        rt_hblk_t *h = (rt_hblk_t *)p;
        size_t g = (size_t)(p - g_hp_arena) >> 4;
        if (!(h->flags & HBF_TTL) || (h->size & 15u) || h->size < 2 * sizeof(rt_hblk_t) || p + h->size > g_hp_top) {
            fprintf(stderr, "[ZHP] heap verify FAILED at +%ld: flags=%x size=%u type=%u (corrupt title)\n", (long)(p - g_hp_arena), (unsigned)h->flags, (unsigned)h->size, (unsigned)h->type);
            abort();
        }
        if (!((g_hp_fr.start[g >> 6] >> (g & 63)) & 1u)) {
            fprintf(stderr, "[ZHP] heap verify FAILED at +%ld: a block header without its START bit (size=%u type=%u) -- the START map is exact or it is nothing (ARCH-GC section 15, CEO-1794)\n",
                (long)(p - g_hp_arena), (unsigned)h->size, (unsigned)h->type);
            abort();
        }
        n++;
        p += h->size;
    }
    for (size_t w = 0; w < nw; w++) bits += __builtin_popcountll(w + 1 == nw && (topg & 63) ? g_hp_fr.start[w] & ((1ull << (topg & 63)) - 1u) : g_hp_fr.start[w]);
    if (bits != n) {
        fprintf(stderr, "[ZHP] heap verify FAILED: %ld START bits below the top against %ld block headers -- a START bit that is no header (ARCH-GC section 15, CEO-1794)\n", bits, n);
        abort();
    }
    {
        size_t hig = (size_t)((g_hp_virgin > g_hp_top ? g_hp_virgin : g_hp_top) - g_hp_arena) >> 4, hw = (hig + 63) >> 6;
        uint64_t above = (topg & 63) ? g_hp_fr.start[nw - 1] & ~((1ull << (topg & 63)) - 1u) : 0;
        for (size_t w = nw; w < hw; w++) above |= g_hp_fr.start[w];
        if (above) { fprintf(stderr, "[ZHP] heap verify FAILED: a START bit at or above the top (ARCH-GC 15.11 (3))\n"); abort(); }
        if (g_gc_in) for (size_t w = 0; w < nw; w++) if (((g_hp_fr.ssum[w >> 6] >> (w & 63)) & 1u) != (g_hp_fr.start[w] != 0)) {
            fprintf(stderr, "[ZHP] heap verify FAILED: the START summary disagrees with START at word %ld (ARCH-GC 15.11 (3))\n", (long)w);
            abort();
        }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_report(void) {
    long win_mb = (long)(g_hp_chunk >> 20), rsv_mb = (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 20) : 0L;
    long win_kb = (long)(g_hp_chunk >> 10);
    {
        const char *x = getenv("SCRIP_GC_EXERCISE");
        const char *st = getenv("SCRIP_GC_STRESS");
        if (x && *x && *x != '0')
            fprintf(stderr,
            "[GC-EXERCISE] arena_kb=%ld arena_mb=%ld reserve_mb=%ld stress=%s collections=%ld blocks=%ld bytes=%ld capped=%ld grew=%ld cap_kb=%ld grows=%ld flips=%ld flip_declined=%ld\n", win_kb,
            win_mb, rsv_mb, (st && *st) ? st : "0", rt_gc_runs_count(), g_hp_blocks, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L, g_hp_capped, (long)(g_hp_grown >> 10),
            (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 10) : 0L, g_hp_grows, g_gc_flips, g_gc_flip_declined);
    }
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    long live = rt_gcheap_verify();
    fprintf(stderr, "[ZHP] arena=%ldKB reserve=%ldMB collections=%ld blocks=%ld(alloc'd)=%ld(walked) bytes=%ld capped=%ld grew=%ldKB cap=%ldKB grows=%ld verify=OK\n", win_kb, rsv_mb,
        rt_gc_runs_count(), g_hp_blocks, live, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L, g_hp_capped, (long)(g_hp_grown >> 10),
        (g_hp_cap_end && g_hp_arena) ? (long)((size_t)(g_hp_cap_end - g_hp_arena) >> 10) : 0L, g_hp_grows);
}
#else
long rt_gcheap_verify(void) { return 0; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_huge_advise(char *a0, char *e0) {
    const char *nh = getenv("SCRIP_NOHUGE");
    if (nh && *nh && *nh != '0') return;
    { uintptr_t a = ((uintptr_t)a0 + 0x1FFFFFu) & ~(uintptr_t)0x1FFFFFu, e = ((uintptr_t)e0) & ~(uintptr_t)0x1FFFFFu; if (e > a) madvise((void *)a, (size_t)(e - a), MADV_HUGEPAGE); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static void rt_gcheap_cap_hit(uint64_t need);
unsigned long long rt_gcheap_cap_bytes(void) { return (g_hp_cap_end && g_hp_arena) ? (unsigned long long)(g_hp_cap_end - g_hp_arena) : 0ULL; }
#endif
_Static_assert(sizeof(long) == 8,
    "THE LINE FOLLOWS THE TOP AND THE WINDOW FOLLOWS THE LIVE SET (cto, 2026-09-23, row gc-heap-growth-...; hq_icon's geddump measured 128 KB timing out past 15 s against 0.115 s at the 4096 KB cap)"
    ": the collection line is recomputed as top plus min(SCRIP_GC_LINE_MB, fifteen sixteenths of what is free -- half until CEO-1264) at init, after every grow and after every collection -- a line f"
    "ixed at half the STARTING window made every poll collect once the live set outgrew it -- and a grow commits the larger of the request and the committed size whenever the last collection left th"
    "e window more than half live, else the request alone; still lazy, still one mprotect per grow, still nothing past the cap");
static void rt_gcheap_line_reset(long head) {
    long fr = (long)(g_hp_end - g_hp_top), h = fr >> 4;
    if (head > fr >> 1) head = fr >> 1;
    if (head > h) h = head;
    g_hp_gcline = g_hp_top + gc_line_span(fr - h);
    gc_fr_line_sync();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_gcheap_grow(uint64_t need) {
    size_t left = (size_t)(g_hp_cap_end - g_hp_end) > GC_ERR_RESERVE ? (size_t)(g_hp_cap_end - g_hp_end) - GC_ERR_RESERVE : 0;
#if RT_DIAG
    if (!left) { rt_gcheap_cap_hit(need); return 0; }
#else
    if (!left) return 0;
#endif
    {
        size_t want = (size_t)need + sizeof(rt_hblk_t), committed = (size_t)(g_hp_end - g_hp_arena);
        want = (want + 0xFFFu) & ~(size_t)0xFFFu;
        if ((size_t)g_hp_live * 2u > committed && want < committed) want = committed;
        if (want < g_hp_chunk) want = g_hp_chunk;
        if (want > left) want = left;
        if (mprotect(g_hp_end, want, PROT_READ | PROT_WRITE) != 0) {
            fprintf(stderr, "[ZHP] lazy commit refused %ld KB at %p under a %ld KB hard cap\n", (long)(want >> 10), (void *)g_hp_end, (long)((g_hp_cap_end - g_hp_arena) >> 10));
            return 0;
        }
        gc_huge_advise(g_hp_end, g_hp_end + want);
        g_hp_end += want;
#if RT_DIAG
        g_hp_grown += (long)want;
        g_hp_grows++;
#endif
        rt_gcheap_line_reset(0);
#if RT_DIAG
        if (getenv("SCRIP_ZETA_TELEM"))
            fprintf(stderr, "[ZHP] lazy commit -> %ld KB of a %ld KB hard cap (committed %ld KB total)\n", (long)((g_hp_end - g_hp_arena) >> 10), (long)((g_hp_cap_end - g_hp_arena) >> 10),
            (long)(g_hp_grown >> 10));
#endif
        return 1;
    }
}
#if RT_DIAG
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_cap_hit(uint64_t need) {
    if (g_hp_capped++) return;
    fprintf(stderr,
        "[ZHP] HARD CAP REACHED: %ld KB is committed, THE CAP IS %ld KB AND THE HEAP DOES NOT EXTEND PAST IT (Lon 2026-09-21, in-chat to the ceo, verbatim: \"Place a hard cap on the GC HEAP. Do not "
        "extend it.\" and \"Do however use the lazy instantiation of memory as the heap grows.\"; ceo CEO-1101). Memory inside the cap is instantiated LAZILY, a page at a time, as the heap grows int"
        "o it; there is NOTHING BEYOND THE CAP -- the old 8x reserve is what let a point DECLARED at 64 KB grow to 2 MB and read as a 2 MB measurement wearing a 64 KB label (ceo CEO-1100, coo COO-13"
        "9). A collection is ARMED for the next safe point and this request wanted %llu bytes; if the run now dies exhausted, read the distance to the next SAFE POINT before reading the size of the "
        "cap.\n", (long)((g_hp_end - g_hp_arena) >> 10), (long)((g_hp_cap_end - g_hp_arena) >> 10), (unsigned long long)need);
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *gcbk_alloc(size_t n);
static inline void gc_start_set(const char *at) __attribute__((always_inline));
static inline void gc_start_set(const char *at) { size_t g = (size_t)(at - g_hp_arena) >> 4; g_hp_fr.start[g >> 6] |= 1ull << (g & 63); g_hp_fr.ssum[g >> 12] |= 1ull << ((g >> 6) & 63); }
static inline int gc_blk_marked(const rt_hblk_t *h) __attribute__((always_inline));
static inline int gc_blk_marked(const rt_hblk_t *h) { size_t g = (size_t)((const char *)h - g_hp_arena) >> 4; return (int)((g_hp_fr.live[g >> 6] >> (g & 63)) & 1u); }
static inline int gc_vis_test_set(const rt_hblk_t *h) __attribute__((always_inline));
static inline int gc_vis_test_set(const rt_hblk_t *h) {
    size_t g = (size_t)((const char *)h - g_hp_arena) >> 4;
    uint64_t bit = 1ull << (g & 63);
    int old = (g_hp_fr.vis[g >> 6] & bit) != 0;
    g_hp_fr.vis[g >> 6] |= bit;
    return old;
}
static inline uint64_t gc_rank(const char *p) __attribute__((always_inline));
static inline uint64_t gc_rank(const char *p) {
    size_t o = (size_t)(p - g_hp_arena), g = o >> 4, w = g >> 6;
    return g_hp_fr.prefix[w] + 16u * (uint64_t)__builtin_popcountll(g_hp_fr.live[w] & ((1ull << (g & 63)) - 1u)) + (o & 15u);
}
static inline char *gc_dest(const rt_hblk_t *h) __attribute__((always_inline));
static inline char *gc_dest(const rt_hblk_t *h) { return g_hp_arena + gc_rank((const char *)h); }
int rt_gc_blk_marked(const rt_hblk_t *h) { return gc_blk_marked(h); }
static void rt_gcheap_init(void) {
    gc_knobs_hoist();
    long mb = (long)GC_HEAP_MB, cap_mb, cap_kb, kb = (long)GC_HEAP_KB;
    { extern void rt_cmdline_switches_apply(void); rt_cmdline_switches_apply(); }
    { const char *e = getenv("SCRIP_HEAP_MB"); if (e && *e) { long v = atol(e); if (v >= 1 && v <= 4096) { mb = v; kb = v * 1024L; } } }
    {
        const char *e = getenv("SCRIP_HEAP_KB");
        if (e && *e) {
            long v = atol(e);
            if (v < (long)GC_HEAP_KB_FLOOR || v > 4096L * 1024L) {
                fprintf(stderr,
                    "[ZHP] SCRIP_HEAP_KB=%ld is outside %d..%ld KB and is REFUSED -- the window is not silently clamped, because a run that grades at a size it was not asked for is a false reading ("
                    "ceo CEO-1095)\n", v, (int)GC_HEAP_KB_FLOOR, 4096L * 1024L);
                abort();
            }
            kb = v;
        }
    }
    cap_kb = (long)GC_HEAP_CAP_KB;
    if (cap_kb < kb) cap_kb = kb;
    { const char *e = getenv("SCRIP_HEAP_MAX_MB"); if (e && *e) { long v = atol(e); cap_kb = (v * 1024L >= kb) ? v * 1024L : kb; } }
    {
        const char *e = getenv("SCRIP_HEAP_CAP_KB");
        if (e && *e) {
            long v = atol(e);
            if (v < kb) {
                fprintf(stderr, "[ZHP] SCRIP_HEAP_CAP_KB=%ld is BELOW the %ld KB window and is REFUSED -- a cap under its own window is not a smaller heap, it is an unstatable one (ceo CEO-1101)\n",
                    v, kb);
                abort();
            }
            cap_kb = v;
        }
    }
    if (g_hp_sw_win_kb > 0) {
        if (g_hp_sw_win_kb < (long)GC_HEAP_KB_FLOOR || g_hp_sw_win_kb > 4096L * 1024L) {
            fprintf(stderr, "[ZHP] -i%ldk is outside %d..%ld KB and is REFUSED (the initial heap window is not silently clamped)\n", g_hp_sw_win_kb, GC_HEAP_KB_FLOOR, 4096L * 1024L);
            abort();
        }
        kb = g_hp_sw_win_kb;
        if (cap_kb < kb) cap_kb = kb;
    }
    if (g_hp_sw_cap_kb > 0) {
        if (g_hp_sw_cap_kb < kb) { fprintf(stderr, "[ZHP] -d%ldk is BELOW the %ld KB initial window (-i) and is REFUSED\n", g_hp_sw_cap_kb, kb); abort(); }
        cap_kb = g_hp_sw_cap_kb;
    }
    cap_mb = (cap_kb + 1023L) / 1024L;
    if (cap_mb < 1) cap_mb = 1;
    {
        size_t rsv = (((size_t)cap_kb << 10) + 0xFFFu) & ~(size_t)0xFFFu;
        if ((rsv >> 3) > (size_t)UINT32_MAX) {
            fprintf(stderr, "[ZHP] a %ld KB hard cap is REFUSED: the collector's page map holds each block's offset in 8-byte units in 32 bits, so the heap reservation stops at 32 GB\n", cap_kb);
            abort();
        }
        void *rv = mmap((void *)0, rsv, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
        if (rv == MAP_FAILED) { fprintf(stderr, "[ZHP] heap mmap failed at the %ld KB hard cap\n", cap_kb); abort(); }
        g_hp_arena = (char *)rv;
        g_hp_cap_end = g_hp_arena + rsv;
        {
            size_t ng = rsv >> 4, nw = (ng + 63) >> 6, ns = (nw + 63) >> 6;
            g_hp_fr.ngran = (long)ng;
            g_hp_fr.live = (uint64_t *)gcbk_alloc(nw * sizeof(uint64_t));
            g_hp_fr.vis = (uint64_t *)gcbk_alloc(nw * sizeof(uint64_t));
            g_hp_fr.start = (uint64_t *)gcbk_alloc(nw * sizeof(uint64_t));
            g_hp_fr.ssum = (uint64_t *)gcbk_alloc(ns * sizeof(uint64_t));
            g_hp_fr.prefix = (uint64_t *)gcbk_alloc(nw * sizeof(uint64_t));
            g_hp_fr.start_abs = g_hp_fr.start - ((uintptr_t)g_hp_arena >> 10);
        }
    }
    g_hp_chunk = (((size_t)kb << 10) + 4095u) & ~(size_t)4095u;
    if (mprotect(g_hp_arena, g_hp_chunk, PROT_READ | PROT_WRITE) != 0) { fprintf(stderr, "[ZHP] heap window commit failed (%ld KB of a %ld MB reserve)\n", kb, cap_mb); abort(); }
    g_hp_top = g_hp_arena;
    g_hp_end = g_hp_arena + g_hp_chunk;
    gc_huge_advise(g_hp_arena, g_hp_end);
    g_hp_virgin = g_hp_arena;
    rt_gcheap_line_reset(0);
    gc_static_segs_init();
#if RT_DIAG
    { const char *b = getenv("SCRIP_GC_BIRTH_LEDGER"); if (b && *b && *b != '0') (void)gc_vac_ledger(); }
#endif
    (void)gc_stack_top();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_gcheap_carve(char *at, uint64_t total, uint16_t type) {
    rt_hblk_t *h = (rt_hblk_t *)at;
    h->fwd = 0;
    h->size = (uint32_t)total;
    h->type = type;
    h->flags = HBF_TTL;
    gc_start_set(at);
    g_hp_fr.alloc_total += (long)total;
    if (type == (uint16_t)DT_S) g_hp_fr.alloc_str += (long)total;
    uint64_t pay = total - sizeof(rt_hblk_t);
    if (g_hp_fr.zfull < 0) g_hp_fr.zfull = 0;
    {
        const int zfull = g_hp_fr.zfull;
        int fresh = !zfull && at >= g_hp_virgin;
        if (at + total > g_hp_virgin) g_hp_virgin = at + total;
        if (fresh) { } else if (!zfull && pay > 32 && (type == (uint16_t)DT_S || type == HB_WSC)) memset((char *)(h + 1) + (pay - 32), 0, 32);
        else memset((void *)(h + 1), 0, (size_t)pay);
#if RT_DIAG
        g_hp_blocks += 1;
        gc_birth_record(at, total, type, __builtin_return_address(1), __builtin_return_address(2), (void **)((void **)__builtin_frame_address(0))[0]);
#endif
        return (void *)(h + 1);
    }
}
#if RT_DIAG
typedef struct { void *ra; uint16_t type; long n; long b; } gc_ah_ra_t;
typedef struct { long n, cap; gc_ah_ra_t e[]; } gc_ah_tab_t;
static long g_ah_tn[512];
static long g_ah_tb[512];
static gc_ah_tab_t *g_ah_ra;
static int g_ah_reg = 0;
static void *gcbk_alloc(size_t n);
static void gcbk_drop(void *p);
__attribute__((visibility("hidden"))) int g_ah_on = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_alloc_hist_report(void) {
    fprintf(stderr, "[AH] per-type (type n bytes):\n");
    for (int t = 0; t < 512; t++) if (g_ah_tn[t]) fprintf(stderr, "[AH] T %d %ld %ld\n", t, g_ah_tn[t], g_ah_tb[t]);
    fprintf(stderr, "[AH] per-callsite (ra type n bytes):\n");
    for (long i = 0; g_ah_ra && i < g_ah_ra->cap; i++) if (g_ah_ra->e[i].n) {
        gc_ah_ra_t *r = &g_ah_ra->e[i];
        Dl_info di;
        const char *sym = (dladdr(r->ra, &di) && di.dli_sname) ? di.dli_sname : "?";
        long off = (dladdr(r->ra, &di) && di.dli_saddr) ? (long)((char *)r->ra - (char *)di.dli_saddr) : 0;
        fprintf(stderr, "[AH] R %p %d %ld %ld  %s+%ld\n", r->ra, (int)r->type, r->n, r->b, sym, off);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int rt_alloc_hist_on(void) {
    if (g_ah_on < 0) { const char *e = getenv("SCRIP_ALLOC_HIST"); g_ah_on = (e && *e && *e != '0') ? 1 : 0; if (g_ah_on && !g_ah_reg) { g_ah_reg = 1; atexit(rt_alloc_hist_report); } }
    return g_ah_on;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_alloc_hist_init(void) { (void)rt_alloc_hist_on(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static gc_ah_ra_t *gc_ah_slot(gc_ah_tab_t *t, void *ra, uint16_t type) {
    long m = t->cap - 1, i = (long)((((uintptr_t)ra >> 3) ^ (uintptr_t)type) & (uintptr_t)m);
    while (t->e[i].n && !(t->e[i].ra == ra && t->e[i].type == type)) i = (i + 1) & m;
    return &t->e[i];
}
void rt_alloc_hist_ra(void *ra, uint16_t type, uint64_t bytes) {
    gc_ah_ra_t *e;
    if (!g_ah_ra || (g_ah_ra->n + 1) * 2 > g_ah_ra->cap) {
        long nc = g_ah_ra ? g_ah_ra->cap * 2 : 8;
        gc_ah_tab_t *nt = (gc_ah_tab_t *)gcbk_alloc(sizeof(gc_ah_tab_t) + (size_t)nc * sizeof(gc_ah_ra_t));
        nt->n = 0;
        nt->cap = nc;
        if (g_ah_ra) { for (long k = 0; k < g_ah_ra->cap; k++) if (g_ah_ra->e[k].n) { *gc_ah_slot(nt, g_ah_ra->e[k].ra, g_ah_ra->e[k].type) = g_ah_ra->e[k]; nt->n++; } gcbk_drop((void *)g_ah_ra); }
        g_ah_ra = nt;
    }
    e = gc_ah_slot(g_ah_ra, ra, type);
    if (!e->n) { e->ra = ra; e->type = type; g_ah_ra->n++; }
    e->n += 1;
    e->b += (long)bytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#else
void rt_alloc_hist_ra(void *ra, uint16_t type, uint64_t bytes) { (void)ra; (void)type; (void)bytes; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_alloc_detax = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes) {
#if RT_DIAG
    if (g_alloc_detax == 1 && g_ah_on <= 0) {
        uint64_t tf = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
        if (g_hp_gcline && !g_gc_in && g_hp_top + tf > g_hp_gcline) g_gc_pending = 1;
        if (g_hp_top + tf <= g_hp_end) { if (g_hp_qlo) gc_quar_release(g_hp_top + tf); { void *rf = rt_gcheap_carve(g_hp_top, tf, type); g_hp_top += tf; return rf; } }
    }
    if (g_ah_on > 0) { unsigned t = (unsigned)type & 511u; g_ah_tn[t] += 1; g_ah_tb[t] += (long)payload_bytes; }
#else
    if (g_alloc_detax == 1) {
        uint64_t tf = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
        if (g_hp_gcline && !g_gc_in && g_hp_top + tf > g_hp_gcline) g_gc_pending = 1;
        if (g_hp_top + tf <= g_hp_end) { { void *rf = rt_gcheap_carve(g_hp_top, tf, type); g_hp_top += tf; return rf; } }
    }
#endif
    uint64_t total = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
    void *r;
#if RT_DIAG
    static long stress_n = -1, stress_c = 0;
    if (!g_hp_report_reg) { g_hp_report_reg = 1; atexit(rt_gcheap_report); }
#else
    if (!g_hp_report_reg) g_hp_report_reg = 1;
#endif
    if (!g_hp_arena) rt_gcheap_init();
#if RT_DIAG
    if (stress_n < 0) { const char *e = getenv("SCRIP_GC_STRESS"); stress_n = e ? atol(e) : 0; }
    if (stress_n > 0 && ++stress_c >= stress_n) { stress_c = 0; g_gc_pending = 1; }
    {
        static long since = 0, budget = -1;
        if (budget < 0) { const char *e = getenv("SCRIP_GC_BUDGET_MB"); long mb = e ? atol(e) : 0; budget = mb > 0 ? (mb << 20) : 0; }
        if (!g_alloc_detax) g_alloc_detax = (stress_n == 0 && budget == 0 && g_ah_on <= 0 && !gc_birth_on() && g_hp_arena && g_hp_report_reg) ? 1 : -1;
        g_hp_fr.armed = (g_alloc_detax == 1 && g_ah_on <= 0 && !gc_birth_on()) ? 1 : 0;
        if (budget) { since += (long)total; if (since >= budget && (g_hp_top - g_hp_arena) * 2 >= (g_hp_end - g_hp_arena)) { since = 0; g_gc_pending = 2; } }
    }
#else
    if (!g_alloc_detax) g_alloc_detax = (g_hp_arena && g_hp_report_reg) ? 1 : -1;
    g_hp_fr.armed = (g_alloc_detax == 1) ? 1 : 0;
#endif
    if (g_hp_gcline && !g_gc_in && g_hp_top + total > g_hp_gcline && g_hp_top + total <= g_hp_end) g_gc_pending = 1;
    if (g_hp_top + total > g_hp_end && g_hp_win + total > g_hp_wend) { g_gc_pending = 1; rt_gcheap_grow(total); }
#if RT_DIAG
    if (g_hp_top + total <= g_hp_end) { if (g_hp_qlo) gc_quar_release(g_hp_top + total); r = rt_gcheap_carve(g_hp_top, total, type); g_hp_top += total; return r; }
#else
    if (g_hp_top + total <= g_hp_end) { r = rt_gcheap_carve(g_hp_top, total, type); g_hp_top += total; return r; }
#endif
    if (g_hp_win + total <= g_hp_wend) {
        uint64_t avail = (uint64_t)(g_hp_wend - g_hp_win);
        if (avail - total == sizeof(rt_hblk_t)) total += sizeof(rt_hblk_t);
        r = rt_gcheap_carve(g_hp_win, total, type);
        g_hp_win += total;
        if (g_hp_win < g_hp_wend) { rt_hblk_t *fl = (rt_hblk_t *)g_hp_win; fl->fwd = 0; fl->size = (uint32_t)(g_hp_wend - g_hp_win); fl->type = HB_FILL; fl->flags = HBF_TTL; gc_start_set(g_hp_win); }
        return r;
    }
    if (g_hp_end + GC_ERR_RESERVE == g_hp_cap_end && mprotect(g_hp_end, GC_ERR_RESERVE, PROT_READ | PROT_WRITE) == 0) { g_hp_end = g_hp_cap_end; rt_gcheap_line_reset(0); }
#if RT_DIAG
    fprintf(stderr,
        "[ZHP] heap exhausted AT THE HARD CAP (%ld KB cap, %ld KB committed lazily so far, %ld blocks live, cap hit %ld times, COLLECTIONS RUN %ld) -- THIS REQUEST wanted %llu payload bytes (%llu wi"
        "th the header) of block kind %u. THE HEAP DOES NOT EXTEND ON LON'S ORDER, so this is one of exactly three things and never a fourth: a CORRUPTED LENGTH (a size computed from a pointer a col"
        "lection moved asks for the world -- read the size FIRST), a MISSING SAFE POINT between this allocation and the last one (the collection is armed and never ran), or a live set that genuinely"
        " does not fit the window THIS RUN NAMED. SCRIP_HEAP_MAX_MB and SCRIP_HEAP_CAP_KB name the CAP ITSELF rather than a reserve above the window. \342\233\224 READ COLLECTIONS RUN FIRST: AN ABOR"
        "T WITH ZERO COLLECTIONS IS NEVER A CAPACITY VERDICT -- nothing was ever reclaimed, so the cap was never full of LIVE data, and the cause is a MISSING SAFE POINT between two allocations. Cap"
        "acity means the collector RAN, reclaimed what it could, and the survivors still did not fit (ceo CEO-1101, the classification the coo's item-4 rail buckets on).\n",
        (long)((g_hp_cap_end - g_hp_arena) >> 10), (long)((g_hp_end - g_hp_arena) >> 10), g_hp_blocks, g_hp_capped, rt_gc_runs_count(), (unsigned long long)payload_bytes, (unsigned long long)total,
        (unsigned)type);
#endif
    {
        extern void rt_heap_out_of_memory(unsigned, unsigned long long, long, long);
        rt_heap_out_of_memory((unsigned)type, (unsigned long long)payload_bytes, (long)((g_hp_cap_end - g_hp_arena) >> 10), (long)((g_hp_end - g_hp_arena) >> 10));
    }
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GC_RET_VOID 0
#define GC_RET_DESCR 1
#define GC_RET_OTHER 2
#define GC_RET_UNKNOWN 3
#include "../../templates/x86/gc_allocating_table.inc"
typedef struct { rt_emitted_regs_t r; uint64_t callee; int hit; } rt_uw_caller_t;
static _Unwind_Reason_Code rt_uw_caller_step(struct _Unwind_Context *c, void *a) {
    extern void rt_icn_line_of_pc(uint64_t pc, long *line, const char **file);
    rt_uw_caller_t *u = (rt_uw_caller_t *)a;
    uint64_t ip = (uint64_t)_Unwind_GetIP(c);
    long line = 0;
    const char *file = (const char *)0;
    if (ip) rt_icn_line_of_pc(ip, &line, &file);
    if (line <= 0) { u->callee = (uint64_t)_Unwind_GetRegionStart(c); return _URC_NO_REASON; }
    u->r.ra = ip;
    u->r.sp = (uint64_t)_Unwind_GetCFA(c);
    u->r.rbx = (uint64_t)_Unwind_GetGR(c, 3);
    u->r.rbp = (uint64_t)_Unwind_GetGR(c, 6);
    u->r.r12 = (uint64_t)_Unwind_GetGR(c, 12);
    u->r.r13 = (uint64_t)_Unwind_GetGR(c, 13);
    u->r.r14 = (uint64_t)_Unwind_GetGR(c, 14);
    u->r.r15 = (uint64_t)_Unwind_GetGR(c, 15);
    u->hit = 1;
    return _URC_END_OF_STACK;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_emitted_caller_of_descr_call(rt_emitted_regs_t *r) {
    extern int rtx_entry_is(const char *sym);
    rt_uw_caller_t u;
    Dl_info di;
    memset(&u, 0, sizeof u);
    _Unwind_Backtrace(rt_uw_caller_step, &u);
    if (!u.hit || !u.callee || !dladdr((void *)(uintptr_t)u.callee, &di) || !di.dli_sname || rtx_entry_is(di.dli_sname)) return 0;
    for (int i = 0; i < GC_ALLOC_TAB_N; i++) if (!strcmp(GC_ALLOC_TAB[i].n, di.dli_sname)) { if (GC_ALLOC_TAB[i].rc != GC_RET_DESCR) return 0; *r = u.r; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *c_rt_str_alloc(long n) {
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)DT_S, 0);
#endif
    long want = (n < 0 ? 0 : n) + 1;
    _Static_assert(DT_S < HB_ZCOL,
        "value-world heap types carry DTYPE_t verbatim: DT_S is the ONLY DTYPE_t ever passed as a block type (every other caller passes HB_*), so the invariant is that it can never be mistaken for o"
        "ne -- it is NOT that DT_S holds any particular value. This assert read DT_S == 1 until s230, which pinned an incidental number instead of the property its own message names, and therefore f"
        "ired on the TAG-3 class-bit renumber while the property still held.");
    return (char *)rt_gcheap_alloc((uint16_t)DT_S, (uint64_t)want);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_str_dup(const char *s) { if (!s) s = ""; long n = (long)strlen(s); char *b = rt_str_alloc(n); if (b) memcpy(b, s, (size_t)n); return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_wsb_alloc(size_t n) {
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSB, (uint64_t)n);
#endif
    return rt_gcheap_alloc((uint16_t)HB_WSB, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_ws_alloc_descr(size_t n) {
    size_t b = (n ? n : 1) * sizeof(DESCR_t);
    void *q;
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_DVEC, (uint64_t)b);
#endif
    q = rt_gcheap_alloc((uint16_t)HB_DVEC, (uint64_t)b);
    if (q) memset(q, 0, b);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_heap_alloc_c(size_t n) {
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_WSC, 0);
#endif
    return rt_gcheap_alloc((uint16_t)HB_WSC, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_struct_alloc(uint16_t type, size_t n) {
    if ((type < HB_PLDB || type > HB_PLDBK) && type != HB_TRIE && type != HB_TRIEN) abort();
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), type, (uint64_t)n);
#endif
    return rt_gcheap_alloc(type, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pvec_alloc(size_t n) {
    size_t b = (n ? n : 1) * sizeof(void *);
    void *q;
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), (uint16_t)HB_PVEC, (uint64_t)b);
#endif
    q = rt_gcheap_alloc((uint16_t)HB_PVEC, (uint64_t)b);
    if (q) memset(q, 0, b);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pvec_realloc(void *p, size_t n) {
    if (!p) return rt_pvec_alloc(n);
    {
        rt_hblk_t *h = (rt_hblk_t *)p - 1;
        size_t old = (size_t)h->size - sizeof(rt_hblk_t), want = (n ? n : 1) * sizeof(void *);
        if (want <= old) return p;
        return rt_gcheap_grow_block(p, (uint16_t)HB_PVEC, (uint64_t)want);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pm_struct_alloc(uint16_t type, size_t n) {
    if (type < HB_DTP || type > HB_DTPRCP) abort();
#if RT_DIAG
    if (rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), type, (uint64_t)n);
#endif
    return rt_gcheap_alloc(type, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_visit_raw(const char **loc);
void *rt_gcheap_grow_block(void *p, uint16_t type, uint64_t n) {
#if RT_DIAG
    static long stress_n = -1;
#endif
    if (!p) { void *q = c_rt_gcheap_alloc(type, n ? n : 1); memset(q, 0, (size_t)(n ? n : 1)); return q; }
    {
        rt_hblk_t *h = (rt_hblk_t *)p - 1;
        uint64_t old = (uint64_t)h->size - sizeof(rt_hblk_t);
        if (n <= old) return p;
#if RT_DIAG
        if (stress_n < 0) { const char *e = getenv("SCRIP_GC_STRESS"); stress_n = e ? atol(e) : 0; }
#endif
        {
            uint64_t total = sizeof(rt_hblk_t) + ((n + 15u) & ~15ull);
            uint64_t delta = total - (uint64_t)h->size;
            if (
#if RT_DIAG
            stress_n == 0 && g_ah_on <= 0 &&
#endif
            !g_gc_in && (char *)h + h->size == g_hp_top && g_hp_top + delta <= g_hp_end && total <= 0xFFFFFFFFull) {
                if (g_hp_gcline && g_hp_top + delta > g_hp_gcline) g_gc_pending = 1;
#if RT_DIAG
                if (g_hp_qlo) gc_quar_release(g_hp_top + delta);
#endif
                memset(g_hp_top, 0, (size_t)delta);
                h->size = (uint32_t)total;
                g_hp_top += delta;
                g_hp_fr.alloc_total += (long)delta;
                return p;
            }
        }
        { void *q = c_rt_gcheap_alloc(type, n); memcpy(q, p, (size_t)old); memset((char *)q + old, 0, (size_t)(n - old)); return q; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_wsb_realloc(void *p, size_t n) { if (!p) return rt_wsb_alloc(n); return rt_gcheap_grow_block(p, (uint16_t)HB_WSB, (uint64_t)n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gv_reserve(gv_t *v, uint16_t kind, uint32_t esz, uint64_t need, const char *name) {
    uint64_t nc;
    if (need <= v->cap) return;
    nc = v->cap ? (uint64_t)v->cap * 2 : 4;
    if (nc < need) nc = need;
    if (esz == 0 || nc > 0x7FFFFFFFull / esz) { fprintf(stderr, "gc_vec: table %s cannot grow to %llu elements of %u bytes\n", name, (unsigned long long)nc, (unsigned)esz); abort(); }
    v->p = rt_gcheap_grow_block(v->p, kind, nc * (uint64_t)esz);
    v->cap = (uint32_t)nc;
    v->esz = esz;
    v->kind = kind;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *gv_push(gv_t *v, uint16_t kind, uint32_t esz, const char *name) {
    void *e;
    gv_reserve(v, kind, esz, (uint64_t)v->len + 1, name);
    e = (char *)v->p + (size_t)v->len * esz;
    memset(e, 0, esz);
    v->len++;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gv_gc_root(gv_t *v) { if (v->p) rt_gc_visit_raw((const char **)&v->p); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_agg_alloc(int kind, size_t n) {
    uint16_t ty = (uint16_t)(HB_AGGV + (kind < 0 ? 0 : (kind > 2 ? 2 : kind)));
#if RT_DIAG
    if (g_alloc_detax != 1 && rt_alloc_hist_on()) rt_alloc_hist_ra(__builtin_return_address(0), ty, (uint64_t)n);
#endif
    return rt_gcheap_alloc(ty, (uint64_t)(n ? n : 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_heap_strdup_c(const char *s) { if (!s) return (char *)0; { size_t n = strlen(s); char *q = (char *)rt_heap_alloc_c(n + 1); memcpy(q, s, n + 1); return q; } }
#include <setjmp.h>
#include "../core/core.h"
#include "rt_coexpr.h"
#include "ct_arena.h"
long g_scrip_coexpr_live;
static struct gc_rng_t { const char *lo, *hi; } *g_gc_rrng = (struct gc_rng_t *)0;
static long g_gc_rrng_n = 0, g_gc_rrng_cap = 0;
static struct gc_seg_t { char *lo, *hi; } *g_gc_segs = (struct gc_seg_t *)0;
static long g_gc_nseg = -1, g_gc_seg_cap = 0;
static rt_hblk_t **g_gc_idx = (rt_hblk_t **)0;
static rt_hblk_t **g_gc_idxbuf = (rt_hblk_t **)0;
static long g_gc_icap = 0;
static long g_gc_nblk = 0;
typedef struct gc_slot_t { const char **loc; const char *val; } gc_slot_t;
typedef struct gc_tr_args_t { const struct gc_rng_t *rr; long n; char *base; const char *oldtop; char *newtop; const rt_hp_fr_t *fr; const char *arena; const char *cap_end; } gc_tr_args_t;
_Static_assert(__builtin_offsetof(gc_tr_args_t, rr) == 0 && __builtin_offsetof(gc_tr_args_t, n) == 8 && __builtin_offsetof(gc_tr_args_t, base) == 16 &&
    __builtin_offsetof(gc_tr_args_t, oldtop) == 24 && __builtin_offsetof(gc_tr_args_t, newtop) == 32 && __builtin_offsetof(gc_tr_args_t, fr) == 40 && __builtin_offsetof(gc_tr_args_t, arena) == 48 &&
    __builtin_offsetof(gc_tr_args_t, cap_end) == 56 && sizeof(struct gc_rng_t) == 16, "rt_gc_trail_run (rtx_gc.s) reads its argument block and the root ranges at these offsets");
extern long rt_gc_first_dead_run(const uint64_t *live, size_t topg);
extern uint64_t rt_gc_prefix_run(uint64_t *prefix, const uint64_t *live, size_t nw);
extern void rt_gc_fix_run(gc_slot_t *slots, long nslot, char *base, size_t logran, const rt_hp_fr_t *fr, const char *arena);
extern char *rt_gc_slide_run(char *base, long fd, const rt_hp_fr_t *fr, const char *arena, const char *top);
extern void rt_gc_start_compact_run(rt_hp_fr_t *fr, const char *arena, const char *base, const char *oldtop);
extern void rt_gc_ssum_run(rt_hp_fr_t *fr, size_t nw);
extern uint64_t rt_gc_trail_run(const gc_tr_args_t *a);
extern void rt_gc_clear_run(rt_hp_fr_t *fr, size_t nw);
typedef struct gc_mk_t {
    DESCR_t **wl;
    long wln, wlcap;
    gc_slot_t *slots;
    long nslot, scap;
    const char *arena, *top;
    const uint64_t *ssum, *start;
    uint64_t *live, *vis;
    rt_hblk_t *mhead;
    DESCR_t *rng_p;
    long rng_n, wlmax, interior, dvec_elems, dvec_nondvec, nmk, mxs, walked, pv_end, draining;
    const char *fc_anchor;
    const void *fc_map;
    const char *fc_lo, *fc_hi;
    long fc_i;
    const char *fc_c, *fc_e;
    long fc_kind;
    const char *sw_p, *sw_hi;
    void **hs;
    long hcap, hn, tb_h, code_unsorted, census, maps, diag, trap, plant, verify, telem, chain;
} gc_mk_t;
_Static_assert(__builtin_offsetof(gc_mk_t, wl) == 0 && __builtin_offsetof(gc_mk_t, wln) == 8 && __builtin_offsetof(gc_mk_t, wlcap) == 16 && __builtin_offsetof(gc_mk_t, slots) == 24 &&
    __builtin_offsetof(gc_mk_t, nslot) == 32 && __builtin_offsetof(gc_mk_t, scap) == 40 && __builtin_offsetof(gc_mk_t, arena) == 48 && __builtin_offsetof(gc_mk_t, top) == 56 &&
    __builtin_offsetof(gc_mk_t, ssum) == 64 && __builtin_offsetof(gc_mk_t, start) == 72 && __builtin_offsetof(gc_mk_t, live) == 80 && __builtin_offsetof(gc_mk_t, vis) == 88 &&
    __builtin_offsetof(gc_mk_t, mhead) == 96 && __builtin_offsetof(gc_mk_t, rng_p) == 104 && __builtin_offsetof(gc_mk_t, rng_n) == 112 && __builtin_offsetof(gc_mk_t, wlmax) == 120 &&
    __builtin_offsetof(gc_mk_t, interior) == 128 && __builtin_offsetof(gc_mk_t, dvec_elems) == 136 && __builtin_offsetof(gc_mk_t, dvec_nondvec) == 144 && __builtin_offsetof(gc_mk_t, nmk) == 152 &&
    __builtin_offsetof(gc_mk_t, mxs) == 160 && __builtin_offsetof(gc_mk_t, walked) == 168 && __builtin_offsetof(gc_mk_t, pv_end) == 176 && __builtin_offsetof(gc_mk_t, draining) == 184 &&
    __builtin_offsetof(gc_mk_t, fc_anchor) == 192 && __builtin_offsetof(gc_mk_t, fc_map) == 200 && __builtin_offsetof(gc_mk_t, fc_i) == 224 && __builtin_offsetof(gc_mk_t, fc_c) == 232 &&
    __builtin_offsetof(gc_mk_t, fc_kind) == 248 && __builtin_offsetof(gc_mk_t, sw_p) == 256 && __builtin_offsetof(gc_mk_t, sw_hi) == 264 && __builtin_offsetof(gc_mk_t, hs) == 272 &&
    __builtin_offsetof(gc_mk_t, hcap) == 280 && __builtin_offsetof(gc_mk_t, hn) == 288 && __builtin_offsetof(gc_mk_t, tb_h) == 296 && __builtin_offsetof(gc_mk_t, code_unsorted) == 304 &&
    __builtin_offsetof(gc_mk_t, census) == 312 && __builtin_offsetof(gc_mk_t, maps) == 320 && __builtin_offsetof(VCELL_t, cellp) == 0 && __builtin_offsetof(VCELL_t, tbl) == 8 &&
    __builtin_offsetof(VCELL_t, key) == 16 && __builtin_offsetof(VCELL_t, key_d) == 24 && __builtin_offsetof(VCELL_t, sv) == 40 && sizeof(gc_frame_map_t) == 32 && GC_LAY_DESCR == 0 &&
    GC_LAY_PTR_GC == 2, "rt_gc_mark_run (rtx_gc.s) reads and writes the mark cursor at these offsets");
_Static_assert(__builtin_offsetof(ARBLK_t, lo) == 0 && __builtin_offsetof(ARBLK_t, hi) == 4 && __builtin_offsetof(ARBLK_t, ndim) == 8 && __builtin_offsetof(ARBLK_t, lo2) == 12 &&
    __builtin_offsetof(ARBLK_t, hi2) == 16 && __builtin_offsetof(ARBLK_t, proto) == 24 && __builtin_offsetof(ARBLK_t, data) == 32 && __builtin_offsetof(TBBLK_t, buckets) == 0 &&
    __builtin_offsetof(TBBLK_t, nbuck) == 8 && __builtin_offsetof(TBBLK_t, dflt) == 32 && __builtin_offsetof(TBBLK_t, ord) == 56 && __builtin_offsetof(TBBLK_t, ord_len) == 64 &&
    __builtin_offsetof(TBBLK_t, icn_ord) == 112 && __builtin_offsetof(TBBUCK_t, len) == 0 && __builtin_offsetof(TBBUCK_t, ent) == 8 && __builtin_offsetof(TBPAIR_t, key_descr) == 0 &&
    __builtin_offsetof(TBPAIR_t, val) == 16 && sizeof(TBPAIR_t) == 40 && __builtin_offsetof(DATINST_t, type) == 0 && __builtin_offsetof(DATINST_t, fields) == 8 &&
    __builtin_offsetof(DATBLK_t, nfields) == 8 && HB_ARR == 212 && HB_AGGP == 207 && HB_AGGT == 208 && HB_AGGB == 211 && HB_PVEC == 220,
    "rt_gc_mark_run (rtx_gc.s) walks arrays, tables, buckets, pairs, data instances and pointer vectors at these offsets and dispatches the move list on these types");
_Static_assert(HB_AGGV == 206 && HB_FILL == 202 && HB_DINST == 213 && HB_DVEC == 214 && DATA_ELEMS_SLEN == 1 && PROCVAL_SLEN == 0xFFFFFFFEu && PROCVAL_BUILTIN_SLEN == 0xFFFFFFFCu &&
    PROCVAL_EXTERNAL_SLEN == 0xFFFFFFFBu, "rt_gc_mark_run (rtx_gc.s) carries these block types and descriptor lengths as its own defines");
__attribute__((visibility("hidden"))) gc_mk_t g_gc_mk;
static void gc_knobs_hoist(void) {
    g_gc_mk.diag = gc_diag_on();
    g_gc_mk.chain = gc_chain_check_on();
#if RT_DIAG
    g_gc_mk.trap = gc_trap_on();
    g_gc_mk.plant = gc_plant_flip();
    g_gc_mk.verify = gc_verify_on();
    g_gc_mk.telem = gc_telem_on();
    g_gc_mk.maps = gc_maps_on();
    g_gc_mk.census = gc_walk_census_on();
#endif
}
_Static_assert(__builtin_offsetof(gc_slot_t, loc) == 0 && __builtin_offsetof(gc_slot_t, val) == 8 && sizeof(gc_slot_t) == 16,
    "the slot record is two words (ARCH-GC section 15.11 (1)): the slot's address and the pointer value read at registration, so the fix is arena plus the rank of that value, idempotent under a doub"
    "le" " registration, with no header read and no slot read; the fix run reads it at these offsets and steps it by 16");
_Static_assert(__builtin_offsetof(rt_hblk_t, fwd) == 0 && __builtin_offsetof(rt_hblk_t, size) == 8 && __builtin_offsetof(rt_hblk_t, flags) == 14 && sizeof(rt_hblk_t) == 16 && HBF_TTL == 1 &&
    HBF_MARK == 2 && HBF_VIS == 0x10, "the collector leaves in rtx_alloc.s read a block header at these offsets and clear MARK|VIS with the mask 0xFFED");
static int g_gc_in = 0;
#if RT_DIAG
static long g_gc_runs = 0, g_gc_interior = 0;
#else
static long g_gc_runs = 0;
#endif
static char *g_gc_stktop = (char *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GCBK_MAGIC 0x5a47424b48445200ull
typedef struct gcbk_head_t { uint64_t magic; uint64_t len; } gcbk_head_t;
static size_t gcbk_page(void) { static size_t pg = 0; if (!pg) pg = (size_t)sysconf(_SC_PAGESIZE); return pg; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *gcbk_alloc(size_t n) {
    size_t pg = gcbk_page(), need = (n + sizeof(gcbk_head_t) + pg - 1) & ~(pg - 1);
    gcbk_head_t *h = (gcbk_head_t *)mmap((void *)0, need, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (h == MAP_FAILED) { fprintf(stderr, "[ZHP] collector bookkeeping mmap failed (%zu bytes)\n", need); abort(); }
    h->magic = GCBK_MAGIC;
    h->len = (uint64_t)need;
    return (void *)((uint8_t *)h + sizeof(gcbk_head_t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static size_t gc_pg(void) { static size_t pg = 0; if (!pg) { long v = sysconf(_SC_PAGESIZE); pg = (v > 0) ? (size_t)v : (size_t)4096; } return pg; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(int) == 4,
    "THE COLLECTOR'S SELF-CHECKS ARE DIAGNOSTICS AND ONE SWITCH TURNS THEM ALL OFF (Lon 2026-09-25, in-chat to the ceo, verbatim: \"Why must there be a heap check at every collection? That seems to "
    "be diagnostic. Can the be turned off by a switch?\" and \"For benchmarks turn off all diagnostic code.\"; ceo CEO-1262): SCRIP_DIAG=0 turns off the heap check at every collection (rt_gcheap_ver"
    "ify), the 0xDB poison of vacated ground, the stale-read quarantine and its ledger, and the compiled node-id stores; each keeps its own switch (SCRIP_GC_VERIFY, SCRIP_GC_POISON, SCRIP_GC_TRAP, S"
    "CRIP_DIAG_REGS), which wins when set; unset, every diagnostic stays on, so every test runs with them and every benchmark driver exports SCRIP_DIAG=0");
#if RT_DIAG
static int gc_diag_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DIAG"); v = (e && *e == '0') ? 0 : 1; } return v; }
#else
static int gc_diag_on(void) { return 0; }
#endif
#if RT_DIAG
static int gc_verify_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_VERIFY"); v = (e && *e) ? (*e != '0') : gc_diag_on(); } return v; }
static int gc_telem_on(void) { static int v = -1; if (v < 0) v = getenv("SCRIP_ZETA_TELEM") ? 1 : 0; return v; }
static int gc_trap_on(void) { static int t = -1; if (t < 0) { const char *e = getenv("SCRIP_GC_TRAP"); t = (e && *e) ? (*e != '0') : gc_diag_on(); } return t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GC_QUAR_RELEASE_BYTES (256u * 1024u)
_Static_assert((GC_QUAR_RELEASE_BYTES & (GC_QUAR_RELEASE_BYTES - 1)) == 0,
    "THE QUARANTINE IS RELEASED IN POWER-OF-TWO CHUNKS AHEAD OF THE ALLOCATOR (ceo 2026-09-25, CEO-1253): a page-granular release cost porter 9136 mprotect calls and 19 ms at the 512 MB demo arena, "
    "one per page the allocator advanced over vacated ground; the trap still faults on every stale read below the released chunk, and the released chunk is at most this many bytes of 0xDB poison ahe"
    "ad of the top");
static void gc_quar_release(char *need_end) {
    size_t pg;
    char *b;
    if (!g_hp_qlo || need_end <= g_hp_qlo) return;
    pg = gc_pg();
    if (pg < GC_QUAR_RELEASE_BYTES) pg = GC_QUAR_RELEASE_BYTES;
    b = (char *)(((uintptr_t)need_end + (pg - 1)) & ~(uintptr_t)(pg - 1));
    if (b > g_hp_qhi) b = g_hp_qhi;
    if (b <= g_hp_qlo) return;
    if (mprotect(g_hp_qlo, (size_t)(b - g_hp_qlo), PROT_READ | PROT_WRITE) != 0) {
        fprintf(stderr,
            "[ZGC-TRAP] could not re-commit vacated ground arena+%ld..arena+%ld for the allocator -- the stale-read trap refuses the run rather than let the mutator write ground the kernel will not "
            "hand back\n", (long)(g_hp_qlo - g_hp_arena), (long)(b - g_hp_arena));
        abort();
    }
    g_hp_qlo = b;
    if (g_hp_qlo >= g_hp_qhi) { g_hp_qlo = (char *)0; g_hp_qhi = (char *)0; g_gc_vacn = 0; }
    gc_fr_line_sync();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_quar_arm(char *lo) {
    size_t pg;
    char *a;
    char *b;
    char *ceil;
    if (!gc_trap_on() || !g_hp_arena) return;
    pg = gc_pg();
    ceil = g_hp_virgin < g_hp_end ? g_hp_virgin : g_hp_end;
    a = (char *)(((uintptr_t)lo + (pg - 1)) & ~(uintptr_t)(pg - 1));
    b = (char *)((uintptr_t)ceil & ~(uintptr_t)(pg - 1));
    if (g_hp_qhi > b) b = g_hp_qhi;
    if (b <= a) return;
    if (mprotect(a, (size_t)(b - a), PROT_NONE) != 0) {
        static int said = 0;
        if (!said) {
            said = 1;
            fprintf(stderr,
                "[ZGC-TRAP] mprotect PROT_NONE refused arena+%ld..arena+%ld -- THE STALE-READ TRAP IS OFF FOR THIS RUN and a stale read will return a plausible 0xDB answer again; a silent instrument"
                " is never evidence of health\n", (long)(a - g_hp_arena), (long)(b - g_hp_arena));
        }
        g_hp_qlo = (char *)0;
        g_hp_qhi = (char *)0;
        return;
    }
    g_hp_qlo = a;
    g_hp_qhi = b;
    g_hp_qgen = g_gc_vacgen;
    g_hp_qarm++;
    g_hp_qbytes += (long)(b - a);
    gc_fr_line_sync();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_birth_on(void) { return (g_gc_vac && g_gc_vac[g_gc_vaccap].size) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static gc_vac_t *gc_vac_ledger(void) {
    if (!g_gc_vac) {
        const char *e = getenv("SCRIP_GC_TRAP_LEDGER");
        const char *k = getenv("SCRIP_GC_BIRTH_LEDGER");
        long c = e && *e ? atol(e) : 65536;
        long b = k && *k ? atol(k) : 0;
        if (c < 64) c = 64;
        if (c > (1L << 22)) c = 1L << 22;
        if (b < 0) b = 0;
        if (b > (1L << 22)) b = 1L << 22;
        g_gc_vac = (gc_vac_t *)gcbk_alloc((size_t)(c + b + 1) * sizeof(gc_vac_t));
        if (!g_gc_vac) return (gc_vac_t *)0;
        g_gc_vaccap = c;
        {
            gc_vac_t *hd = &g_gc_vac[c];
            hd->at = (char *)0;
            hd->fwd = (char *)0;
            hd->size = (uint32_t)b;
            hd->gen = 0;
            hd->type = 0;
            hd->pad = 0;
            hd->serial = 0;
            hd->ra_site = (void *)0;
            hd->ra_from = (void *)0;
        }
    }
    return g_gc_vac;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_birth_beyond_the_allocators(void **fp, void **site, void **from) {
    char *top = gc_stack_top();
    void *ra = *from;
    for (int k = 0; k < 6 && ra && fp; k++) {
        Dl_info di;
        if (!dladdr(ra, &di) || !di.dli_sname || !strstr(di.dli_sname, "alloc")) { *from = ra; return; }
        { void **nx = (void **)fp[0]; if ((char *)nx <= (char *)fp || (char *)nx + 2 * sizeof(void *) > top) { *from = ra; return; } *site = ra; fp = nx; ra = fp[1]; }
    }
    *from = ra;
}
static void gc_birth_record(char *at, uint64_t total, uint16_t type, void *ra_site, void *ra_from, void **fp_from) {
    gc_vac_t *hd;
    long b, s;
    if (!g_gc_vac) return;
    hd = &g_gc_vac[g_gc_vaccap];
    b = (long)hd->size;
    if (b <= 0) return;
    s = hd->serial++;
    gc_birth_beyond_the_allocators((void **)fp_from[0], &ra_site, &ra_from);
    {
        gc_vac_t *v = &g_gc_vac[g_gc_vaccap + 1 + (s % b)];
        v->at = at;
        v->fwd = (char *)0;
        v->size = (uint32_t)total;
        v->gen = 0;
        v->type = type;
        v->pad = 0;
        v->serial = s;
        v->ra_site = ra_site;
        v->ra_from = ra_from;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_birth_find(char *at) {
    long i, b;
    if (!g_gc_vac || !at) return -1;
    b = (long)g_gc_vac[g_gc_vaccap].size;
    if (b <= 0) return -1;
    {
        long best = -1;
        for (i = 1; i <= b; i++) { gc_vac_t *v = &g_gc_vac[g_gc_vaccap + i]; if (v->gen == 0 && v->size && v->at == at && (best < 0 || v->serial > g_gc_vac[best].serial)) best = g_gc_vaccap + i; }
        return best;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_birth_cover(char *f) {
    long i, b, best = -1;
    if (!g_gc_vac || !f) return -1;
    b = (long)g_gc_vac[g_gc_vaccap].size;
    if (b <= 0) return -1;
    for (i = 1; i <= b; i++) {
        gc_vac_t *v = &g_gc_vac[g_gc_vaccap + i];
        if (v->gen == 0 && v->size && f >= v->at && f < v->at + (long)v->size && (best < 0 || v->serial > g_gc_vac[best].serial)) best = g_gc_vaccap + i;
    }
    return best;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *gc_chain_back(long hit, long *hops) {
    long i, cur = hit, k = 0;
    char *a = g_gc_vac[hit].at;
    for (;;) {
        long prev = -1;
        for (i = g_gc_vacn - 1; i >= 0; i--) if (g_gc_vac[i].fwd == a && g_gc_vac[i].gen < g_gc_vac[cur].gen) { prev = i; break; }
        if (prev < 0 || k >= 4096) break;
        cur = prev;
        a = g_gc_vac[cur].at;
        k++;
    }
    *hops = k;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_vac_record(char *base, char *vlo) {
    long i, need = 0;
    if (!gc_trap_on()) return;
    if (!gc_vac_ledger()) return;
    { long lo = 0, hi = g_gc_nblk; while (lo < hi) { long m = (lo + hi) >> 1; if ((char *)g_gc_idx[m] < vlo) lo = m + 1; else hi = m; } need = g_gc_nblk - lo; i = lo; }
    if (need > g_gc_vaccap - g_gc_vacn) { g_gc_vacn = 0; g_gc_vacover++; }
    g_gc_vacgen = g_gc_runs + 1;
    for (; i < g_gc_nblk; i++) {
        rt_hblk_t *h = g_gc_idx[i];
        if (g_gc_vacn >= g_gc_vaccap) break;
        {
            gc_vac_t *v = &g_gc_vac[g_gc_vacn++];
            v->at = (char *)h;
            v->fwd = gc_blk_marked(h) ? base + gc_rank((const char *)h) : (char *)0;
            v->size = h->size;
            v->gen = (uint32_t)g_gc_vacgen;
            v->type = h->type;
            v->pad = 0;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(long long) == 8,
    "A REGISTER HOLDING THE 0xDB POISON AT A FAULT IS A POINTER READ FROM VACATED GROUND (cto 2026-09-23, CTO-155): the collector fills the ground it vacates with 0xDB, a pointer loaded from there i"
    "s 0xDBDB..., non-canonical, and the kernel reports a general-protection fault with si_addr 0 -- so the stale-address report above cannot see it and the crash printed nothing; the registers carr"
    "y the evidence instead");
static void gc_say(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
static void gc_say(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    { int len = vfmt_len(fmt, ap); char b[len]; ssize_t w; vsnprintf(b, (size_t)len, fmt, ap); w = write(2, b, (size_t)len - 1); (void)w; }
    va_end(ap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_gc_poison_reg_report(const long long *gregs, int n, void *ip) {
    static const char *const rn[] = { "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rdi", "rsi", "rbp", "rbx", "rdx", "rax", "rcx", "rsp", "rip" };
    int k, hit = 0;
    for (k = 0; k < n && k < 16; k++) {
        uint64_t v = (uint64_t)gregs[k];
        if ((v >> 32) == 0xDBDBDBDBu) {
            if (!hit) gc_say("[ZGC-STALE] fault at instruction %p with a register holding the 0xDB POISON the collector writes over vacated ground:", ip);
            hit = 1;
            gc_say(" %s=%#llx", rn[k], (unsigned long long)v);
        }
    }
    if (!hit) return 0;
    gc_say(
        "\n[ZGC-STALE]   a POINTER WAS READ FROM VACATED GROUND and dereferenced: the block that held it was moved or reclaimed by collection #%ld and whatever still points there was NEVER VISITED%s"
        "\n", g_gc_runs, g_gc_in ? " -- AND THE FAULT HAPPENED INSIDE A COLLECTION, so the stale holder is a root the previous collection did not repair" : "");
    return 1;
}
int rt_gc_stale_addr_report(void *fault, void *ip) {
    char *f = (char *)fault;
    long off, i, hit = -1;
    if (!g_hp_arena || f < g_hp_arena || f >= g_hp_cap_end) return 0;
    off = (long)(f - g_hp_arena);
    gc_say("[ZGC-STALE] SIGSEGV touching GC heap ground at %p (arena+%ld) from instruction %p -- a STALE HEAP POINTER was used, not a wild address\n", fault, off, ip);
    if ((g_hp_qlo && f >= g_hp_qlo && f < g_hp_qhi) || (g_hp_flo && f >= g_hp_flo && f < g_hp_fhi)) {
        gc_say("[ZGC-STALE]   this page is QUARANTINED PROT_NONE: collection #%ld (or an earlier one) vacated arena+%ld..arena+%ld and no allocation has taken it back; the live top is arena+%ld\n",
            g_hp_qgen, (long)(g_hp_qlo - g_hp_arena), (long)(g_hp_qhi - g_hp_arena), (long)(g_hp_top - g_hp_arena));
        for (i = g_gc_vacn - 1; i >= 0; i--) if (f >= g_gc_vac[i].at && f < g_gc_vac[i].at + g_gc_vac[i].size) { hit = i; break; }
        if (hit >= 0 && g_gc_vac[hit].fwd)
            gc_say(
            "[ZGC-STALE]   the block that lived here: #%ld kind=%u size=%u at arena+%ld, +%ld into it -- it was MOVED to arena+%ld by collection #%ld, so the holder of this pointer was NEVER VISITED"
            " and kept the pre-move address\n", hit, (unsigned)g_gc_vac[hit].type, (unsigned)g_gc_vac[hit].size, (long)(g_gc_vac[hit].at - g_hp_arena), (long)(f - g_gc_vac[hit].at),
            (long)(g_gc_vac[hit].fwd - g_hp_arena), (long)g_gc_vac[hit].gen);
        else if (hit >= 0)
            gc_say(
            "[ZGC-STALE]   the block that lived here: #%ld kind=%u size=%u at arena+%ld, +%ld into it -- it was RECLAIMED by collection #%ld because nothing marked it, so the holder of this pointer "
            "was never visited either\n", hit, (unsigned)g_gc_vac[hit].type, (unsigned)g_gc_vac[hit].size, (long)(g_gc_vac[hit].at - g_hp_arena), (long)(f - g_gc_vac[hit].at), (long)g_gc_vac[hit].gen)
            ;
        else gc_say(
            "[ZGC-STALE]   no block in the vacated ledger covers this address (ledger holds %ld entries through collection #%ld, %ld resets) -- this ground was vacated before the ledger's oldest ent"
            "ry, so the block is not nameable from here\n", g_gc_vacn, g_gc_vacgen, g_gc_vacover);
        {
            long hops = 0;
            char *born = (char *)0;
            long bk = -1;
            int viachain = 0;
            if (hit >= 0) { born = gc_chain_back(hit, &hops); bk = gc_birth_find(born); viachain = bk >= 0; }
            if (bk < 0) { bk = gc_birth_cover(f); if (bk >= 0) { born = g_gc_vac[bk].at; hops = 0; } }
            if (bk >= 0) {
                Dl_info d0, d1;
                gc_vac_t *v = &g_gc_vac[bk];
                int k0 = dladdr(v->ra_site, &d0), k1 = v->ra_from ? dladdr(v->ra_from, &d1) : 0;
                const char *s0 = (k0 && d0.dli_sname) ? d0.dli_sname : "?";
                const char *s1 = (k1 && d1.dli_sname) ? d1.dli_sname : "?";
                const char *m0 = (k0 && d0.dli_fname) ? d0.dli_fname : "?";
                const char *m1 = (k1 && d1.dli_fname) ? d1.dli_fname : "?";
                long f0 = k0 ? (long)((char *)v->ra_site - (char *)d0.dli_fbase) : 0L, f1 = k1 ? (long)((char *)v->ra_from - (char *)d1.dli_fbase) : 0L;
                gc_say("[ZGC-BIRTH]   block #%ld, kind=%u/%s, size=%u, allocated by %s (%s+0x%lx) from %s (%s+0x%lx), born at arena+%ld, +%ld into it, %s\n", v->serial, (unsigned)v->type,
                    HB_KIND_NAME(v->type), (unsigned)v->size, s0, m0, f0, s1, m1, f1, (long)(born - g_hp_arena), (long)(f - born),
                    viachain ? "reached by walking the vacated ledger back through its relocations" : "matched by address in the birth ring, with no vacated-ledger entry to relocate it");
                if (viachain) gc_say("[ZGC-BIRTH]   it was relocated %ld time(s) between that birth and the collection that vacated the ground you just read\n", hops);
            } else if (!gc_birth_on())
                gc_say(
                "[ZGC-BIRTH]   THE BIRTH LEDGER IS OFF, so this fault names an address and not a defect -- set SCRIP_GC_BIRTH_LEDGER=<ring entries> and the report will name the block's serial, its t"
                "ype at birth and the site that allocated it\n");
            else gc_say(
                "[ZGC-BIRTH]   the birth ledger does not hold this block: the chain walked back %ld relocation(s) to arena+%ld and no birth record survives there (ring holds %ld entries, %ld births "
                "so far) -- raise SCRIP_GC_BIRTH_LEDGER or the record has been overwritten\n", hops, born ? (long)(born - g_hp_arena) : -1L, (long)g_gc_vac[g_gc_vaccap].size,
                g_gc_vac[g_gc_vaccap].serial);
        }
    } else if (f >= g_hp_top)
        gc_say(
        "[ZGC-STALE]   the address is above the live top (arena+%ld) but its page is not quarantined: vacated ground the allocator has already taken back, or ground beyond the committed window (aren"
        "a+%ld)\n", (long)(g_hp_top - g_hp_arena), (long)(g_hp_end - g_hp_arena));
    else gc_say("[ZGC-STALE]   the address is BELOW the live top (arena+%ld), so it is inside live heap and this fault is NOT the vacated-ground trap\n", (long)(g_hp_top - g_hp_arena));
    if (g_gc_in) gc_say("[ZGC-STALE]   THE FAULT HAPPENED INSIDE A COLLECTION -- read this as a defect in the collector or in the trap before reading it as a mutator stale pointer\n");
    gc_say(
        "[ZGC-STALE]   a stale read means a ROOT THE COLLECTOR NEVER VISITED kept a pre-collection address; the instruction above is where it was used, not where it was stored. SCRIP_GC_TRAP=0 resto"
        "res the old silent wrong answer.\n");
    return 1;
}
#else
int rt_gc_poison_reg_report(const long long *gregs, int n, void *ip) { (void)gregs; (void)n; (void)ip; return 0; }
int rt_gc_stale_addr_report(void *fault, void *ip) { (void)fault; (void)ip; return 0; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *gcbk_grow(void *p, size_t n) {
    gcbk_head_t *h;
    size_t pg, need;
    void *nv;
    if (!p) return gcbk_alloc(n);
    h = (gcbk_head_t *)((uint8_t *)p - sizeof(gcbk_head_t));
    if (h->magic != GCBK_MAGIC) { fprintf(stderr, "[ZHP] gcbk_grow on a block the collector bookkeeping allocator did not hand out (%p)\n", p); abort(); }
    pg = gcbk_page();
    need = (n + sizeof(gcbk_head_t) + pg - 1) & ~(pg - 1);
    if (need <= (size_t)h->len) return p;
    nv = mremap((void *)h, (size_t)h->len, need, MREMAP_MAYMOVE);
    if (nv == MAP_FAILED) { fprintf(stderr, "[ZHP] collector bookkeeping mremap failed (%zu to %zu bytes)\n", (size_t)h->len, need); abort(); }
    h = (gcbk_head_t *)nv;
    h->len = (uint64_t)need;
    return (void *)((uint8_t *)h + sizeof(gcbk_head_t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gcbk_drop(void *p) {
    gcbk_head_t *h;
    if (!p) return;
    h = (gcbk_head_t *)((uint8_t *)p - sizeof(gcbk_head_t));
    if (h->magic != GCBK_MAGIC) { fprintf(stderr, "[ZHP] gcbk_drop on a block the collector bookkeeping allocator did not hand out (%p)\n", p); abort(); }
    h->magic = 0;
    munmap((void *)h, (size_t)h->len);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static void *gcbk_reserve(void *p, size_t need) {
    size_t have = p ? (size_t)((gcbk_head_t *)((uint8_t *)p - sizeof(gcbk_head_t)))->len - sizeof(gcbk_head_t) : 0;
    if (need <= have) return p;
    return gcbk_grow(p, need > 2 * have ? need : 2 * have);
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static double gc_walk_ns(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (double)t.tv_sec * 1e9 + (double)t.tv_nsec; }
#endif
typedef struct { const void *pc; const char *r; const void *rbp; } gc_ent_t;
static long gc_collect_ex(const gc_ent_t *ent);
static char *g_gc_seam_sp = (char *)0;
static DESCR_t *g_gc_shield_arr = (DESCR_t *)0;
static int g_gc_shield_n = 0;
static const char **g_gc_shield_r = (const char **)0;
static int g_gc_shield_r_is_probe = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_point_arr_run(DESCR_t *arr, int n, const char **r0, char *floor, int is_probe, const gc_ent_t *ent) {
    int pv = g_gc_pending;
    if (!pv && g_hp_gcline && g_hp_top > g_hp_gcline) pv = 1;
    if (!pv) return;
    g_gc_pending = 0;
    g_gc_shield_arr = arr;
    g_gc_shield_n = n;
    g_gc_shield_r = r0;
    g_gc_shield_r_is_probe = is_probe;
    g_gc_seam_sp = floor;
    gc_collect_ex(ent);
    g_gc_seam_sp = (char *)0;
    g_gc_shield_arr = (DESCR_t *)0;
    g_gc_shield_n = 0;
    g_gc_shield_r = (const char **)0;
    g_gc_shield_r_is_probe = 0;
}
static __attribute__((used, noinline)) void gc_point_arr_body(DESCR_t *arr, int n, const char **r0, char *floor, int is_probe) {
    gc_ent_t ent = { floor ? *(const void *const *)floor : (const void *)0, floor ? floor + 8 : (const char *)0, *(const void *const *)__builtin_frame_address(0) };
    gc_point_arr_run(arr, n, r0, floor, is_probe, &ent);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point_arr_c(DESCR_t *arr, int n, const char **r0, char *floor) {
#if RT_DIAG
    g_gc_poll_site = (const char *)__builtin_return_address(0);
#endif
    gc_ent_t ent = { __builtin_return_address(0), (const char *)__builtin_frame_address(0) + 16, *(const void *const *)__builtin_frame_address(0) };
    gc_point_arr_run(arr, n, r0, floor, 0, &ent);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point_arr_probe_c(DESCR_t *arr, int n, const char **saved_subject_reg, char *floor) {
    gc_ent_t ent = { __builtin_return_address(0), (const char *)__builtin_frame_address(0) + 16, *(const void *const *)__builtin_frame_address(0) };
    gc_point_arr_run(arr, n, saved_subject_reg, floor, 1, &ent);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_point(DESCR_t *d0, const char **r0) { rt_gc_point_arr(d0, d0 ? 1 : 0, r0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(void *) == 8,
    "THE VISITED SET HAS TWO KEY SPACES IN ONE TABLE AND THEY MUST NOT COLLIDE (cto 2026-09-23, CTO-153; procedure_every_scan_replace_9 CRASH at every stress point once the assignment seam was gone)"
    ": bit 0 is RETIRED (cfo 2026-10-01: a registered slot is no longer a key -- each slot record carries its target offset, so the fixup writes the same word however often a slot is registered, and"
    " the per-slot probe was a cache miss per pointer), bit 1 marks a NAME-REFERENCED CELL (DT_N slen=1, the 16 bytes at d->ptr pushed to the worklist once), and an even key now marks only a TABLE R"
    "EACHED FROM A VARIABLE CELL, the cheap first filter of gc_visit_vcell (cfo 2026-10-01: every heap aggregate whose contents were visited -- table, array, record instance, variable cell -- carrie"
    "s HBF_VIS in its own header instead, cleared with HBF_MARK at the index phase). A DT_N cell reference whose ptr lands on a table's first word inserted the table's own address, every later DT_T "
    "visit read it as already visited, the buckets array was never marked, and the collection after next walked reclaimed ground -- the key spaces are distinct by construction now, and gc_visit_one'"
    "s DT_N slen=1 case is the only writer of bit 1");
static inline int gc_hins(void *p) __attribute__((always_inline));
static void gc_hins_grow(void) {
    long ncap = g_gc_mk.hcap ? g_gc_mk.hcap * 2 : 4096;
    void **nh = (void **)gcbk_alloc((size_t)ncap * sizeof(void *));
    if (!nh) { fprintf(stderr, "[ZGC] visited-set alloc failed\n"); abort(); }
    for (long i = 0; i < g_gc_mk.hcap; i++) if (g_gc_mk.hs[i]) {
        uint64_t h = ((uint64_t)g_gc_mk.hs[i] >> 4) * 0x9E3779B97F4A7C15ull;
        long s = (long)(h & (uint64_t)(ncap - 1));
        while (nh[s]) s = (s + 1) & (ncap - 1);
        nh[s] = g_gc_mk.hs[i];
    }
    gcbk_drop(g_gc_mk.hs);
    g_gc_mk.hs = nh;
    g_gc_mk.hcap = ncap;
}
static inline int gc_hins(void *p) {
    if (g_gc_mk.hn * 10 >= g_gc_mk.hcap * 7) gc_hins_grow();
    {
        uint64_t h = ((uint64_t)p >> 4) * 0x9E3779B97F4A7C15ull;
        long s = (long)(h & (uint64_t)(g_gc_mk.hcap - 1));
        while (g_gc_mk.hs[s]) { if (g_gc_mk.hs[s] == p) return 0; s = (s + 1) & (g_gc_mk.hcap - 1); }
        g_gc_mk.hs[s] = p;
        g_gc_mk.hn++;
        return 1;
    }
}
#if RT_DIAG
static long g_gc_dvec_elems = 0;
static long g_gc_dvec_nondvec = 0;
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline rt_hblk_t *gc_blk_of(const char *p) __attribute__((always_inline));
static inline rt_hblk_t *gc_blk_of(const char *p) {
    size_t g, w;
    uint64_t m;
    if (!p || p < g_hp_arena || p >= g_hp_top) return (rt_hblk_t *)0;
    g = (size_t)(p - g_hp_arena) >> 4;
    w = g >> 6;
    m = g_hp_fr.start[w] & ((2ull << (g & 63)) - 1u);
    if (!m) {
        size_t sw;
        if (!w) return (rt_hblk_t *)0;
        w--;
        sw = w >> 6;
        m = g_hp_fr.ssum[sw] & ((2ull << (w & 63)) - 1u);
        while (!m) { if (!sw) return (rt_hblk_t *)0; sw--; m = g_hp_fr.ssum[sw]; }
        w = (sw << 6) + (63 - (size_t)__builtin_clzll(m));
        m = g_hp_fr.start[w];
    }
    { rt_hblk_t *h = (rt_hblk_t *)(g_hp_arena + (((w << 6) + (63 - (size_t)__builtin_clzll(m))) << 4)); return (p < (char *)h + h->size) ? h : (rt_hblk_t *)0; }
}
static inline void gc_live_set(size_t g, size_t n) __attribute__((always_inline));
static inline void gc_live_set(size_t g, size_t n) {
    uint64_t *w = g_hp_fr.live + (g >> 6);
    size_t b = g & 63;
    if (b + n <= 64) { *w |= (n == 64 ? ~0ull : ((1ull << n) - 1u)) << b; return; }
    *w++ |= ~0ull << b;
    n -= 64 - b;
    while (n >= 64) { *w++ = ~0ull; n -= 64; }
    if (n) *w |= (1ull << n) - 1u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int gc_type_moves(uint16_t t) __attribute__((always_inline));
#define GC_TYPE_MOVES(t) ((t) == HB_DVEC || ((t) >= HB_PLDB && (t) <= HB_TRIEN) || (t) == HB_ARR || (t) == HB_DINST || HB_IS_AGG(t))
static inline int gc_type_moves(uint16_t t) { return GC_TYPE_MOVES(t) ? 1 : 0; }
#define GC_MK_BIT(k) (GC_TYPE_MOVES(HB_AGGV + (k)) ? 1u << (k) : 0u)
_Static_assert((GC_MK_BIT(0) | GC_MK_BIT(1) | GC_MK_BIT(2) | GC_MK_BIT(3) | GC_MK_BIT(4) | GC_MK_BIT(5) | GC_MK_BIT(6) | GC_MK_BIT(7) | GC_MK_BIT(8) | GC_MK_BIT(9) | GC_MK_BIT(10) | GC_MK_BIT(11) |
    GC_MK_BIT(12) | GC_MK_BIT(13) | GC_MK_BIT(14) | GC_MK_BIT(15) | GC_MK_BIT(16) | GC_MK_BIT(17) | GC_MK_BIT(18)) == 0x7FDE7u && !GC_TYPE_MOVES(HB_AGGV - 1) && !GC_TYPE_MOVES(HB_AGGV + 19),
    "rt_gc_mark_run (rtx_gc.s) tests a block type for moving as bit (type - HB_AGGV) of its MK_MOVES 0x7FDE7, so every moving type lies in HB_AGGV..HB_AGGV+18 and the set reads as that mask");
static inline void gc_mark_blk(rt_hblk_t *h, uint16_t addf) __attribute__((always_inline));
static inline void gc_mark_blk(rt_hblk_t *h, uint16_t addf) {
    size_t g = (size_t)((char *)h - g_hp_arena) >> 4;
    if (h->type == HB_FILL) return;
    if (!((g_hp_fr.live[g >> 6] >> (g & 63)) & 1u)) {
        gc_live_set(g, (size_t)h->size >> 4);
        g_gc_mk.nmk++;
        if ((long)h->size > g_gc_mk.mxs) g_gc_mk.mxs = (long)h->size;
        if (gc_type_moves(h->type)) { h->fwd = (uint64_t)(uintptr_t)g_gc_mk.mhead; g_gc_mk.mhead = h; }
    }
    if (addf) g_hp_fr.vis[g >> 6] |= 1ull << (g & 63);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_visit_segment(const char *lo0, const char *hi0, const gc_ent_t *ent, const scrip_coctx_t *sc, int run);
static int gc_frames_visit_check(uint64_t *v);
static void gc_frames_visit_judge(const uint64_t *v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_slot_grow(void) {
    g_gc_mk.scap = g_gc_mk.scap ? g_gc_mk.scap * 2 : 4096;
    g_gc_mk.slots = (gc_slot_t *)gcbk_grow((void *)g_gc_mk.slots, (size_t)g_gc_mk.scap * sizeof(*g_gc_mk.slots));
    if (!g_gc_mk.slots) abort();
}
static inline void gc_slot_reg_tgt(void *loc, rt_hblk_t *tgt) __attribute__((always_inline));
static inline void gc_slot_reg_tgt(void *loc, rt_hblk_t *tgt) {
    if (g_gc_mk.nslot == g_gc_mk.scap) gc_slot_grow();
    g_gc_mk.slots[g_gc_mk.nslot].loc = (const char **)loc;
    g_gc_mk.slots[g_gc_mk.nslot].val = *(const char *const *)loc;
    (void)tgt;
    g_gc_mk.nslot++;
}
static inline void gc_slot_reg(void *loc) __attribute__((always_inline));
static inline void gc_slot_reg(void *loc) { const char *v = *(const char *const *)loc; rt_hblk_t *t = gc_blk_of(v); if (!t) return; gc_slot_reg_tgt(loc, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gc_visit_raw_c(const char **loc) { rt_hblk_t *h = gc_blk_of(*loc); if (!h) return; gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)loc, h); }
void rt_gc_visit_raw(const char **loc) { extern int rt_gc_visit_raw_a(const char **); if (!rt_gc_visit_raw_a(loc)) rt_gc_visit_raw_c(loc); }
void rt_gc_visit_raw_in(const char **loc, const void *holder) { extern int rt_gc_visit_raw_a(const char **); (void)holder; if (!rt_gc_visit_raw_a(loc)) rt_gc_visit_raw_c(loc); }
int rt_gc_weak_keep(const char **loc) { rt_hblk_t *h = gc_blk_of(*loc); if (!h) return 1; if (!gc_blk_marked(h)) return 0; gc_slot_reg((void *)loc); return 1; }
int rt_gc_ptr_in_heap_slot(const char *p) { return gc_blk_of(p) != (rt_hblk_t *)0; }
int rt_gc_in_arena(const char *p) { return (g_hp_arena && p >= g_hp_arena && p < g_hp_top) ? 1 : 0; }
int rt_gc_slot_registered(const void *loc) { for (long i = 0; i < g_gc_mk.nslot; i++) if ((const void *)g_gc_mk.slots[i].loc == loc) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_mark_agg(const void *p) { rt_hblk_t *h = gc_blk_of((const char *)p); if (h) gc_mark_blk(h, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int gc_tag_bears_ptr(uint8_t v) __attribute__((always_inline));
static inline int gc_tag_bears_ptr(uint8_t v) {
    return v == DT_S || v == DT_SNUL || v == DT_X || v == DT_A || v == DT_T || v == DT_N || v == DT_DATA || v == DT_P || v == DT_PLVAR || v == DT_PLREF || v == DT_E || v == DT_BIG || v == DT_CPLX ||
        v == DT_EXTL;
}
static void gc_wl_grow(void) {
    g_gc_mk.wlcap = g_gc_mk.wlcap ? g_gc_mk.wlcap * 2 : 4096;
    g_gc_mk.wl = (DESCR_t **)gcbk_grow((void *)g_gc_mk.wl, (size_t)g_gc_mk.wlcap * sizeof(*g_gc_mk.wl));
    if (!g_gc_mk.wl) { fprintf(stderr, "[ZGC] mark worklist alloc failed at %ld entries\n", g_gc_mk.wlcap); abort(); }
}
static inline void gc_wl_push(DESCR_t *d) __attribute__((always_inline));
static inline void gc_wl_push(DESCR_t *d) {
    if (!d || !gc_tag_bears_ptr(d->v)) return;
    if (g_gc_mk.wln == g_gc_mk.wlcap) gc_wl_grow();
    g_gc_mk.wl[g_gc_mk.wln++] = d;
#if RT_DIAG
    if (g_gc_mk.wln > g_gc_mk.wlmax) g_gc_mk.wlmax = g_gc_mk.wln;
#endif
}
static void gc_visit_tbblk(struct _TBBLK_t *t);
static int gc_block_exact(const char *q, uint16_t want_type);
static rt_hblk_t *gc_block_exact_h(const char *q, uint16_t want_type);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline uint16_t gc_mark_vis(rt_hblk_t *h) __attribute__((always_inline));
static inline uint16_t gc_mark_vis(rt_hblk_t *h) {
    size_t g = (size_t)((char *)h - g_hp_arena) >> 4;
    uint64_t bit = 1ull << (g & 63);
    uint16_t old = (uint16_t)(((g_hp_fr.live[g >> 6] & bit) ? HBF_MARK : 0) | ((g_hp_fr.vis[g >> 6] & bit) ? HBF_VIS : 0));
    if (!(old & HBF_MARK)) { gc_live_set(g, (size_t)h->size >> 4); g_gc_mk.nmk++; if ((long)h->size > g_gc_mk.mxs) g_gc_mk.mxs = (long)h->size; }
    g_hp_fr.vis[g >> 6] |= bit;
    return old;
}
static void gc_visit_vcell(VCELL_t *vc) {
    gc_mark_agg(vc);
    if (vc->key) gc_mark_agg(vc->key);
    gc_slot_reg((void *)&vc->key);
    gc_slot_reg((void *)&vc->tbl);
    gc_slot_reg((void *)&vc->cellp);
    if (vc->cellp) gc_mark_agg((const void *)vc->cellp);
    if (vc->tbl && gc_hins((void *)vc->tbl)) { rt_hblk_t *tb = gc_block_exact_h((const char *)vc->tbl, HB_AGGT); if (!tb || !gc_vis_test_set(tb)) gc_visit_tbblk(vc->tbl); }
    rt_gc_visit_descr(&vc->key_d);
    rt_gc_visit_descr(&vc->sv);
    if (vc->cellp) rt_gc_visit_descr(vc->cellp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_tbblk(struct _TBBLK_t *t) {
    gc_mark_agg(t);
    rt_gc_visit_descr(&t->dflt);
    if (t->ord) { rt_gc_visit_raw((const char **)&t->ord); for (unsigned i = 0; i < t->ord_len; i++) rt_gc_visit_descr(&t->ord[i]); }
    if (t->icn_ord) rt_gc_visit_raw((const char **)&t->icn_ord);
    if (!t->buckets) return;
    rt_gc_visit_raw((const char **)&t->buckets);
    for (unsigned b = 0; b < t->nbuck; b++) {
        TBBUCK_t *bk = t->buckets[b];
        if (!bk) continue;
        rt_gc_visit_raw((const char **)&t->buckets[b]);
        for (unsigned i = 0; i < bk->len; i++) { TBPAIR_t *e = &bk->ent[i]; rt_gc_visit_descr(&e->key_descr); rt_gc_visit_descr(&e->val); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_one(DESCR_t *d) {
    if (!d) return;
    switch ((int)d->v) {
        case DT_S:
        case DT_SNUL:
        case DT_X:
        {
            rt_hblk_t *h = gc_blk_of(d->s);
            if (!h) return;
            gc_mark_blk(h, 0);
#if RT_DIAG
            if (d->s != (char *)(h + 1)) g_gc_interior++;
#endif
            gc_slot_reg_tgt((void *)&d->s, h);
            return;
        }
        case DT_N:
        {
            if (d->slen == 2) {
                VCELL_t *vc = (VCELL_t *)d->p;
                rt_hblk_t *vh = gc_block_exact_h((const char *)vc, HB_AGGV);
                if (!vh) return;
                gc_slot_reg_tgt((void *)&d->p, vh);
                if (gc_vis_test_set(vh)) return;
                gc_visit_vcell(vc);
                return;
            }
            if (d->slen == 1) {
                DESCR_t *tc = (DESCR_t *)d->ptr;
                rt_hblk_t *th = gc_blk_of((const char *)tc);
                if (!th || (const char *)tc < (const char *)(th + 1) || (const char *)tc + 16 > (const char *)th + th->size) return;
                gc_slot_reg_tgt((void *)&d->ptr, th);
                gc_mark_agg((const void *)tc);
                if (gc_hins((void *)((uintptr_t)tc | 2u))) gc_wl_push(tc);
                return;
            }
            { rt_hblk_t *h = gc_blk_of(d->s); if (h) { gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)&d->s, h); } }
            return;
        }
        case DT_P:
        { rt_hblk_t *h = gc_blk_of((const char *)d->p); if (!h) return; gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)&d->p, h); return; }
        case DT_PLVAR:
        case DT_PLREF:
        {
            rt_hblk_t *h = gc_blk_of((const char *)d->p);
            if (!h) return;
            gc_mark_blk(h, 0);
#if RT_DIAG
            if ((const char *)d->p != (const char *)(h + 1)) g_gc_interior++;
#endif
            gc_slot_reg_tgt((void *)&d->p, h);
            return;
        }
        case DT_BIG:
        { rt_hblk_t *h = gc_blk_of((const char *)d->p); if (!h) return; gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)&d->p, h); return; }
        case DT_CPLX:
        { rt_hblk_t *h = gc_blk_of((const char *)d->p); if (!h) return; gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)&d->p, h); return; }
        case DT_EXTL:
        { rt_hblk_t *h = gc_blk_of((const char *)d->p); if (!h) return; gc_mark_blk(h, 0); gc_slot_reg_tgt((void *)&d->p, h); return; }
        case DT_E:
        {
            if (!IS_PROCVAL_fn(*d)) return;
            rt_hblk_t *h = gc_blk_of(d->s);
            if (!h) return;
            gc_mark_blk(h, 0);
#if RT_DIAG
            if (d->s != (char *)(h + 1)) g_gc_interior++;
#endif
            gc_slot_reg_tgt((void *)&d->s, h);
            return;
        }
        default:
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_visit_mblk(rt_hblk_t *h) {
    if (h->type == HB_AGGV) { if (!gc_vis_test_set(h)) gc_visit_vcell((VCELL_t *)(h + 1)); return; }
    if (h->type == HB_TRIE || h->type == HB_TRIEN) { extern void pl_trie_gc_visit(uint16_t, void *); pl_trie_gc_visit(h->type, (void *)(h + 1)); return; }
#if RT_DIAG
    if (h->type <= HB_ZCOL || h->type == HB_FILL || h->type == HB_ZBLK || h->type == HB_WSC || h->type == HB_WSB) return;
    g_gc_interior += (long)(((size_t)h->size - sizeof(rt_hblk_t)) / sizeof(void *));
#endif
}
static void gc_wl_drain(void) {
    extern DESCR_t *rt_gc_mark_run(gc_mk_t *);
    for (;;) {
        DESCR_t *d = rt_gc_mark_run(&g_gc_mk);
        if (!d) break;
        if (d == (DESCR_t *)1) { if (g_gc_mk.rng_n) gc_wl_grow(); else gc_slot_grow(); } else if (d == (DESCR_t *)2) gc_wl_grow();
        else if (d == (DESCR_t *)6) gc_hins_grow();
        else if ((uintptr_t)d & 2u) gc_visit_mblk((rt_hblk_t *)((uintptr_t)d & ~(uintptr_t)2u));
        else gc_visit_one(d);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_run_verdicts(DESCR_t *v) {
    extern DESCR_t *rt_gc_mark_run(gc_mk_t *);
    while (v) {
        if (v == (DESCR_t *)1) { if (g_gc_mk.rng_n) gc_wl_grow(); else gc_slot_grow(); } else if (v == (DESCR_t *)2) gc_wl_grow();
        else if (v == (DESCR_t *)6) gc_hins_grow();
        else if ((uintptr_t)v & 2u) gc_visit_mblk((rt_hblk_t *)((uintptr_t)v & ~(uintptr_t)2u));
        else gc_visit_one(v);
        v = rt_gc_mark_run(&g_gc_mk);
    }
    g_gc_mk.draining = 0;
}
void rt_gc_visit_descr(DESCR_t *d) {
    extern DESCR_t *rt_gc_visit_descr_a(DESCR_t *);
    for (;;) {
        DESCR_t *v = rt_gc_visit_descr_a(d);
        if (!v) { g_gc_mk.draining = 0; return; }
        if (v == (DESCR_t *)3) return;
        if (v == (DESCR_t *)4) { gc_wl_grow(); continue; }
        gc_run_verdicts(v);
        return;
    }
}
typedef struct gc_trail_roots_args_t { const struct gc_rng_t *rr; long n; long pos_i; const char *pos_e; long n_hb; const char *top; } gc_trail_roots_args_t;
_Static_assert(__builtin_offsetof(gc_trail_roots_args_t, rr) == 0 && __builtin_offsetof(gc_trail_roots_args_t, n) == 8 && __builtin_offsetof(gc_trail_roots_args_t, pos_i) == 16 &&
    __builtin_offsetof(gc_trail_roots_args_t, pos_e) == 24 && __builtin_offsetof(gc_trail_roots_args_t, n_hb) == 32 && __builtin_offsetof(gc_trail_roots_args_t, top) == 40 &&
    sizeof(pl_tr_entry_t) == 32 && PL_TR_HEADER_BYTES == 64 && __builtin_offsetof(pl_tr_entry_t, pad) == 8 && __builtin_offsetof(pl_tr_entry_t, old) == 16,
    "rt_gc_trail_roots_run (rtx_gc.s) reads its argument block and the trail at these offsets");
void rt_gc_run_proc_roots(gc_roots_args_t *a) { extern long rt_gc_proc_roots_run(gc_roots_args_t *); while (rt_gc_proc_roots_run(a)) gc_slot_grow(); }
void rt_gc_run_flags_roots(gc_roots_args_t *a) {
    extern long rt_gc_flags_roots_run(gc_roots_args_t *);
    for (;;) { long r = rt_gc_flags_roots_run(a); if (!r) break; if (r == 1) gc_slot_grow(); else gc_wl_grow(); }
}
typedef struct gc_ptr_fields_args_t { const char *base; long n; long stride; long pos; long noff; const long *off; long k; } gc_ptr_fields_args_t;
_Static_assert(__builtin_offsetof(gc_ptr_fields_args_t, base) == 0 && __builtin_offsetof(gc_ptr_fields_args_t, n) == 8 && __builtin_offsetof(gc_ptr_fields_args_t, stride) == 16 &&
    __builtin_offsetof(gc_ptr_fields_args_t, pos) == 24 && __builtin_offsetof(gc_ptr_fields_args_t, noff) == 32 && __builtin_offsetof(gc_ptr_fields_args_t, off) == 40 &&
    __builtin_offsetof(gc_ptr_fields_args_t, k) == 48, "rt_gc_ptr_fields_run (rtx_gc.s) reads its argument block at these offsets");
void rt_gc_run_ptr_fields(const void *base, long n, long stride, long noff, const long *off) {
    extern long rt_gc_ptr_fields_run(gc_ptr_fields_args_t *);
    gc_ptr_fields_args_t a = { (const char *)base, n, stride, 0, noff, off, 0 };
    if (noff < 1 || noff > 5) rt_bomb("rt_gc_run_ptr_fields: a run visits one to five pointer fields per entry");
    while (rt_gc_ptr_fields_run(&a)) gc_slot_grow();
}
typedef struct gc_loc_list_args_t { const char *const *const *list; long n; long pos; } gc_loc_list_args_t;
_Static_assert(__builtin_offsetof(gc_loc_list_args_t, list) == 0 && __builtin_offsetof(gc_loc_list_args_t, n) == 8 && __builtin_offsetof(gc_loc_list_args_t, pos) == 16,
    "rt_gc_loc_list_run (rtx_gc.s) reads its argument block at these offsets");
void rt_gc_run_loc_list(const char *const *const *list, long n) {
    extern long rt_gc_loc_list_run(gc_loc_list_args_t *);
    gc_loc_list_args_t a = { list, n, 0 };
    while (rt_gc_loc_list_run(&a)) gc_slot_grow();
}
void rt_gc_run_roots(gc_roots_args_t *a) {
    extern long rt_gc_core_roots_run(gc_roots_args_t *);
    for (;;) { long r = rt_gc_core_roots_run(a); if (!r) break; if (r == 1) gc_slot_grow(); else gc_wl_grow(); }
}
_Static_assert(__builtin_offsetof(gc_roots_args_t, vbk) == 0 && __builtin_offsetof(gc_roots_args_t, vnb) == 8 && __builtin_offsetof(gc_roots_args_t, fbk) == 16 &&
    __builtin_offsetof(gc_roots_args_t, fnb) == 24 && __builtin_offsetof(gc_roots_args_t, udef) == 32 && __builtin_offsetof(gc_roots_args_t, pos_t) == 40 &&
    __builtin_offsetof(gc_roots_args_t, pos_e) == 48 && __builtin_offsetof(gc_roots_args_t, pos_b) == 56 && __builtin_offsetof(gc_roots_args_t, tmp_end) == 64 &&
    __builtin_offsetof(gc_roots_args_t, tmp_p) == 72, "rt_gc_core_roots_run (rtx_gc.s) reads its argument block at these offsets");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static long g_gc_rrng_ss = 0;
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add(const char *lo, const char *hi) {
    if (g_gc_rrng_n == g_gc_rrng_cap) {
        g_gc_rrng_cap = g_gc_rrng_cap ? g_gc_rrng_cap * 2 : 64;
        g_gc_rrng = (struct gc_rng_t *)gcbk_grow((void *)g_gc_rrng, (size_t)g_gc_rrng_cap * sizeof(*g_gc_rrng));
        if (!g_gc_rrng) abort();
    }
    g_gc_rrng[g_gc_rrng_n].lo = lo;
    g_gc_rrng[g_gc_rrng_n].hi = hi;
    g_gc_rrng_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const gc_frame_map_t **g_gc_maps = (const gc_frame_map_t **)0;
static int g_gc_maps_n = 0, g_gc_maps_cap = 0;
#if RT_DIAG
static void gc_frame_maps_dump_atexit(void);
#endif
static int gc_frame_map_registered(const gc_frame_map_t *m) { for (int i = 0; i < g_gc_maps_n; i++) if (g_gc_maps[i] == m) return 1; return 0; }
void rt_gc_frame_maps_add(const gc_frame_map_t *m) {
#if RT_DIAG
    { static int dreg = 0; if (!dreg) { const char *e = getenv("SCRIP_GC_MAPS_DUMP"); dreg = 1; if (e && *e == '1') atexit(gc_frame_maps_dump_atexit); } }
#endif
    if (!m || gc_frame_map_registered(m)) return;
    if (m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-MAP] rt_gc_frame_maps_add: %p is not a frame map (magic %08x)\n", (const void *)m, m->magic); abort(); }
    if (g_gc_maps_n == g_gc_maps_cap) {
        g_gc_maps_cap = g_gc_maps_cap ? g_gc_maps_cap * 2 : 64;
        g_gc_maps = (const gc_frame_map_t **)gcbk_grow((void *)g_gc_maps, (size_t)g_gc_maps_cap * sizeof(*g_gc_maps));
        if (!g_gc_maps) abort();
    }
    g_gc_maps[g_gc_maps_n++] = m;
}
void rt_gc_frame_maps_drop_range(const void *lo, const void *hi) {
    int w = 0;
    for (int i = 0; i < g_gc_maps_n; i++) if ((const void *)g_gc_maps[i] < lo || (const void *)g_gc_maps[i] >= hi) g_gc_maps[w++] = g_gc_maps[i];
    g_gc_maps_n = w;
}
typedef struct { uint64_t off; uint16_t kind, rule; int32_t depth; } gc_site_ent_t;
#define GC_SITE_RULE(e) ((int)((e)->rule & 255u))
#define GC_SITE_EXT(e) ((int)((e)->rule >> 8))
_Static_assert(sizeof(gc_site_ent_t) == 16 && __builtin_offsetof(gc_site_ent_t, kind) == 8 && __builtin_offsetof(gc_site_ent_t, rule) == 10 && __builtin_offsetof(gc_site_ent_t, depth) == 12,
    "A SITE ENTRY IS LAID OUT EXACTLY AS THE EMITTED (pc, word) PAIR, word = kind | rule << 16 | depth << 32 little-endian (cto, row gc-a-registered-site-table-costs-its-entries-not-an-mmapped-page-"
    "per-blo" "ck): a table the emitter wrote sorted by pc with no skipped entry is searched in place in the fragment's own data, so registering it costs no page; any other table is copied as before")
    ;
typedef struct { const uint64_t *tab; const gc_frame_map_t *map; uint64_t lo, hi, maxhi; int n; int ckt; int own; uint64_t link; const gc_site_ent_t *ents; uint64_t code; } gc_site_blk_t;
_Static_assert(sizeof(gc_site_blk_t) == 80 && sizeof(gc_site_ent_t) == 16 && __builtin_offsetof(gc_site_blk_t, lo) == 16 && __builtin_offsetof(gc_site_blk_t, hi) == 24 &&
    __builtin_offsetof(gc_site_blk_t, maxhi) == 32 && __builtin_offsetof(gc_site_blk_t, n) == 40 && __builtin_offsetof(gc_site_blk_t, ents) == 64,
    "rt_gc_site_find_a (rtx_gc.s) searches the site blocks and their entries at these offsets");
static gc_site_blk_t *g_gc_sblk = (gc_site_blk_t *)0;
static int g_gc_sblk_n = 0, g_gc_sblk_cap = 0;
static int gc_site_ent_cmp(const void *a, const void *b) { uint64_t x = ((const gc_site_ent_t *)a)->off, y = ((const gc_site_ent_t *)b)->off; return x < y ? -1 : x > y ? 1 : 0; }
static void gc_site_blk_maxhi_from(int i) {
    for (int j = i < 0 ? 0 : i; j < g_gc_sblk_n; j++) g_gc_sblk[j].maxhi = (j > 0 && g_gc_sblk[j - 1].maxhi > g_gc_sblk[j].hi) ? g_gc_sblk[j - 1].maxhi : g_gc_sblk[j].hi;
}
static void gc_site_blk_add1(const void *tab, int dedup_n, int fix_maxhi) {
    const uint64_t *t = (const uint64_t *)tab;
    int n;
    const gc_frame_map_t *m;
    const gc_site_ent_t *e;
    int k = 0;
    int own = 0;
    int inplace = 1;
    if (!t) return;
    for (int i = 0; i < dedup_n; i++) if (g_gc_sblk[i].tab == t) return;
    n = (int)t[0];
    m = (const gc_frame_map_t *)(uintptr_t)t[1];
    if (m && m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-SITES] rt_gc_frame_sites_add: table %p names no frame map (%p)\n", tab, (const void *)m); abort(); }
    for (int i = 0; i < n && inplace; i++) { uint64_t pc = t[4 + 2 * i], w = t[5 + 2 * i]; if (!pc || (w & 255u) == 4u || (w & 255u) == 5u || (i > 0 && pc <= t[2 + 2 * i])) inplace = 0; }
    if (inplace) {
        e = n > 0 ? (const gc_site_ent_t *)(const void *)(t + 4) : (const gc_site_ent_t *)0;
        k = n;
    } else {
        gc_site_ent_t *c = n > 0 ? (gc_site_ent_t *)gcbk_alloc((size_t)n * sizeof *c) : (gc_site_ent_t *)0;
        for (int i = 0; i < n; i++) {
            uint64_t pc = t[4 + 2 * i], w = t[5 + 2 * i];
            if (!pc) continue;
            if ((w & 255u) == 4u || (w & 255u) == 5u) continue;
            c[k].off = pc;
            c[k].kind = (uint16_t)(w & 0xFFFFu);
            c[k].rule = (uint16_t)((w >> 16) & 0xFFFFu);
            c[k].depth = (int32_t)(uint32_t)(w >> 32);
            k++;
        }
        if (k > 1) qsort(c, (size_t)k, sizeof *c, gc_site_ent_cmp);
        e = c;
        own = c ? 1 : 0;
    }
    if (g_gc_sblk_n == g_gc_sblk_cap) {
        g_gc_sblk_cap = g_gc_sblk_cap ? g_gc_sblk_cap * 2 : 64;
        g_gc_sblk = (gc_site_blk_t *)gcbk_grow((void *)g_gc_sblk, (size_t)g_gc_sblk_cap * sizeof *g_gc_sblk);
        if (!g_gc_sblk) abort();
    }
    {
        int i = g_gc_sblk_n;
        uint64_t code = t[3], lo = k > 0 ? code + e[0].off : code, hi = k > 0 ? code + e[k - 1].off + 1 : code;
        if (code && code < lo) lo = code;
        while (i > 0 && g_gc_sblk[i - 1].lo > lo) { g_gc_sblk[i] = g_gc_sblk[i - 1]; i--; }
        g_gc_sblk[i].tab = t;
        g_gc_sblk[i].map = m;
        g_gc_sblk[i].lo = lo;
        g_gc_sblk[i].hi = hi;
        g_gc_sblk[i].code = code;
        if (code && lo != code) g_gc_mk.code_unsorted = 1;
        g_gc_sblk[i].n = k;
        g_gc_sblk[i].ckt = m ? 0 : (int)t[2];
        g_gc_sblk[i].link = m ? t[2] : 0u;
        g_gc_sblk[i].own = own;
        g_gc_sblk[i].ents = e;
        g_gc_sblk_n++;
        if (fix_maxhi) gc_site_blk_maxhi_from(i);
    }
}
void rt_gc_frame_sites_add(const void *tab) { gc_site_blk_add1(tab, g_gc_sblk_n, 1); }
void rt_gc_frame_sites_drop_range(const void *lo, const void *hi) {
    int w = 0;
    for (int i = 0; i < g_gc_sblk_n; i++) {
        if ((const void *)g_gc_sblk[i].tab < lo || (const void *)g_gc_sblk[i].tab >= hi) { g_gc_sblk[w++] = g_gc_sblk[i]; continue; }
        if (g_gc_sblk[i].own && g_gc_sblk[i].ents) gcbk_drop((void *)(uintptr_t)g_gc_sblk[i].ents);
    }
    g_gc_sblk_n = w;
    gc_site_blk_maxhi_from(0);
}
void rt_gc_frame_sites_install_counted(const void *tab) {
    const uint64_t *t = (const uint64_t *)tab;
    int n0 = g_gc_sblk_n;
    if (!t) return;
    for (int i = 0; i < (int)t[0]; i++) gc_site_blk_add1((const void *)(uintptr_t)t[1 + i], n0, 0);
    gc_site_blk_maxhi_from(0);
}
extern const gc_site_blk_t *rt_gc_site_find_a(uint64_t pc, const gc_site_ent_t **eo, const gc_site_blk_t *blk, long n);
#define gc_site_find(pc, eo) rt_gc_site_find_a((pc), (eo), g_gc_sblk, (long)g_gc_sblk_n)
long rt_gc_frame_sites_count(void) { long n = 0; for (int b = 0; b < g_gc_sblk_n; b++) n += g_gc_sblk[b].n; return n; }
void rt_gc_frame_maps_install(const gc_frame_map_t *const *maps, int n) { for (int i = 0; i < n; i++) rt_gc_frame_maps_add(maps[i]); }
void rt_gc_frame_maps_install_counted(const void *tab) {
    const uint64_t *t = (const uint64_t *)tab;
    int n;
    const gc_frame_map_t *const *maps;
    if (!t) return;
    n = (int)t[0];
    maps = (const gc_frame_map_t *const *)(t + 1);
    rt_gc_frame_maps_add((const gc_frame_map_t *)0);
    if (g_gc_maps_n != 0 || n <= 0) { rt_gc_frame_maps_install(maps, n); return; }
    {
        int nc = g_gc_maps_cap ? g_gc_maps_cap : 64;
        while (nc < n) nc *= 2;
        if (nc != g_gc_maps_cap) { g_gc_maps = (const gc_frame_map_t **)gcbk_grow((void *)g_gc_maps, (size_t)nc * sizeof(*g_gc_maps)); if (!g_gc_maps) abort(); g_gc_maps_cap = nc; }
    }
    for (int i = 0; i < n; i++) {
        const gc_frame_map_t *m = maps[i];
        if (!m) continue;
        if (m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-MAP] rt_gc_frame_maps_install_counted: %p is not a frame map (magic %08x)\n", (const void *)m, m->magic); abort(); }
        g_gc_maps[g_gc_maps_n++] = m;
    }
}
const gc_frame_map_t *const *rt_gc_frame_maps(int *n) { if (n) *n = g_gc_maps_n; return g_gc_maps; }
#if RT_DIAG
void rt_gc_frame_maps_dump(void) {
    int n = 0;
    const gc_frame_map_t *const *t = rt_gc_frame_maps(&n);
    fprintf(stderr, "[GC-MAPTAB] n=%d\n", n);
    for (int i = 0; i < n; i++) {
        const gc_frame_map_t *m = t[i];
        if (!m || m->magic != GC_FRAME_MAP_MAGIC) { fprintf(stderr, "[GC-MAPTAB] BAD entry %d %p\n", i, (const void *)m); continue; }
        fprintf(stderr, "[GC-MAPTAB] graph=%s frame_bytes=%u header_bytes=%u flags=%u value_bytes=%u\n", m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, m->flags,
            (unsigned)m->value_bytes);
        if (m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT)) {
            const uint64_t *t = (const uint64_t *)(m + 1);
            long k = (long)t[0];
            fprintf(stderr, "[GC-MAPTAB-LAYOUT] graph=%s n=%ld", m->graph_name ? m->graph_name : "?", k);
            for (long j = 0; j < k; j++) fprintf(stderr, " %d:%u:%d", GC_LAY_OFF(t[1 + j]), GC_LAY_KIND(t[1 + j]), GC_LAY_SIZE(t[1 + j]));
            fprintf(stderr, "\n");
        }
    }
}
static void gc_frame_maps_dump_atexit(void) { rt_gc_frame_maps_dump(); }
#else
void rt_gc_frame_maps_dump(void) { }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add_topword(const char *lo) { rt_gc_root_range_add(lo, (const char *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_add_seamsafe(const char *lo, const char *hi) {
#if RT_DIAG
    rt_gc_root_range_add(lo, hi);
    g_gc_rrng_ss++;
#else
    rt_gc_root_range_add(lo, hi);
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_range_del(const char *lo) { for (long i = 0; i < g_gc_rrng_n; i++) if (g_gc_rrng[i].lo == lo) { g_gc_rrng[i] = g_gc_rrng[--g_gc_rrng_n]; return; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_phdr_cb(struct dl_phdr_info *info, size_t sz, void *data) {
    const char *nm = info->dlpi_name;
    (void)sz;
    (void)data;
    if (nm && nm[0] && !strstr(nm, "libscrip")) return 0;
    for (int i = 0; i < (int)info->dlpi_phnum; i++) {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD || !(ph->p_flags & PF_W)) continue;
        if (g_gc_nseg == g_gc_seg_cap) {
            g_gc_seg_cap = g_gc_seg_cap ? g_gc_seg_cap * 2 : 16;
            g_gc_segs = (struct gc_seg_t *)gcbk_grow((void *)g_gc_segs, (size_t)g_gc_seg_cap * sizeof(*g_gc_segs));
            if (!g_gc_segs) abort();
        }
        g_gc_segs[g_gc_nseg].lo = (char *)(info->dlpi_addr + ph->p_vaddr);
        g_gc_segs[g_gc_nseg].hi = g_gc_segs[g_gc_nseg].lo + ph->p_memsz;
        g_gc_nseg++;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_static_segs_init(void) { if (g_gc_nseg >= 0) return; g_gc_nseg = 0; dl_iterate_phdr(gc_phdr_cb, (void *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gcheap_warmup(void) { gc_static_segs_init(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void *__libc_stack_end;
_Static_assert(sizeof(void *) == 8,
    "THE MAIN STACK IS THE MAPPING HOLDING __libc_stack_end, NOT THE MAPPING LABELLED [stack] (cto, 2026-09-24, row gc-the-main-stack-top-is-the-mapping-holding-libc-stack-end-...; hq_icon's and hq_"
    "prolog's findings): under valgrind the [stack] line of /proc/self/maps is the HOST's stack while the client's frames live in an unlabelled mapping, so a walk from the mutator's floor to the lab"
    "elled top crossed unmapped ground and every collecting run died rc=139 in the frame walk; glibc's __libc_stack_end is the entry rsp in both settings and the mapping containing it is the real st"
    "ac" "k in both; the label is the fallback only when no mapping contains it");
static int gc_maps_line(FILE *f, unsigned long *a, unsigned long *b, char *pm, int *is_stack) {
    static const char want[] = "[stack]";
    int c, k = 0;
    *is_stack = 0;
    if (fscanf(f, "%lx-%lx %7s", a, b, pm) != 3) { while ((c = fgetc(f)) != EOF && c != 10) { } return c == EOF ? -1 : 0; }
    while ((c = fgetc(f)) != EOF && c != 10) { if (*is_stack) continue; if (c == want[k]) { if (!want[++k]) *is_stack = 1; } else k = (c == want[0]) ? 1 : 0; }
    return 1;
}
static void gc_stack_region(char **lo, char **hi) {
#if RT_DIAG
    FILE *f = fopen("/proc/self/maps", "r");
    unsigned long a = 0, b = 0, la = 0, lb = 0, e = g_gc_stktop ? (unsigned long)(g_gc_stktop - 1) : (unsigned long)__libc_stack_end;
#else
    FILE *f = fopen("/proc/self/maps", "r");
    unsigned long a = 0, b = 0, la = 0, lb = 0, e = g_gc_stktop ? (unsigned long)(g_gc_stktop - 1) : (unsigned long)__libc_stack_end;
#endif
    if (f) {
        int r, st;
        char pm[8];
        unsigned long x = 0, y = 0;
        while ((r = gc_maps_line(f, &x, &y, pm, &st)) >= 0) { if (!r) continue; if (e && x <= e && e < y) { a = x; b = y; break; } if (!lb && st) { la = x; lb = y; } }
        fclose(f);
    }
    if (!b) { a = la; b = lb; }
    *lo = (char *)a;
    *hi = b ? (char *)b : (char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_stack_top_adopt(char *hi) { g_gc_stktop = hi; }
void rt_gc_main_stack_bounds(char **lo, char **hi) { if (!g_gc_stktop) { *lo = *hi = (char *)0; return; } gc_stack_region(lo, hi); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *gc_stack_top(void) { if (g_gc_stktop) return g_gc_stktop; { char *lo, *hi; gc_stack_region(&lo, &hi); g_gc_stktop = hi ? hi : (char *)&lo; } return g_gc_stktop; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct gc_seg_it_t { int stage; pthread_t self, mainthr; int co; scrip_coctx_t *c; char *floor; char *run_hi; char *prbp; const scrip_coctx_t *sc, *run_c; } gc_seg_it_t;
#if RT_DIAG
static long g_gc_co_ctxs, g_gc_co_sigma, g_gc_co_parked;
static int g_gc_rep_pop;
#endif
static void gc_coexpr_records(void) {
#if RT_DIAG
    g_gc_co_ctxs = g_gc_co_sigma = 0;
    scrip_co_gc_visit_records(&g_gc_co_ctxs, &g_gc_co_sigma);
#else
    scrip_co_gc_visit_records((long *)0, (long *)0);
#endif
}
static void gc_seg_begin(gc_seg_it_t *it, char *floor) {
    it->stage = 0;
    it->prbp = (char *)0;
    it->sc = (const scrip_coctx_t *)0;
    it->run_c = (const scrip_coctx_t *)0;
    it->self = pthread_self();
    it->co = scrip_co_main_known(&it->mainthr);
    it->c = it->co ? scrip_co_gc_head() : (scrip_coctx_t *)0;
#if RT_DIAG
    it->floor = floor;
    it->run_hi = g_gc_stktop;
    g_gc_co_parked = 0;
#else
    it->floor = floor;
    it->run_hi = g_gc_stktop;
#endif
}
static char *g_gc_program_top = (char *)0;
char *rt_gc_program_top_get(void) { return g_gc_program_top; }
void rt_gc_program_top_set(char *top) { g_gc_program_top = top; }
static int g_gc_seg_main = 0;
static const gc_frame_map_t *gc_root_map(void) {
    for (int i = g_gc_maps_n - 1; i >= 0; i--) if (g_gc_maps[i] && (g_gc_maps[i]->flags & GC_FRAME_MAP_ROOT)) return g_gc_maps[i];
    return (const gc_frame_map_t *)0;
}
void rt_program_top_adopt(char *entry_sp) {
    const gc_frame_map_t *r = gc_root_map();
    if (!r || !entry_sp || g_gc_program_top) return;
    g_gc_program_top = entry_sp + ((r->flags & GC_FRAME_MAP_ICN_PROC) ? 0L : (long)r->value_bytes + 16 + (long)r->header_bytes);
}
void rt_program_top_adopt_top(char *top) { if (!gc_root_map() || !top || g_gc_program_top) return; g_gc_program_top = top; }
char *rt_gc_program_stack_top(void) { return g_gc_program_top ? g_gc_program_top : gc_stack_top(); }
_Static_assert(sizeof(void *) == 8,
    "A PARKED STACK IS A SEGMENT FROM ITS RECORDED STACK POINTER, NOT FROM ITS MAPPING (cto, 2026-09-23, row gc-heap-growth-...; law CEO-812 THE COLLECTOR GUESSES NOTHING): scrip_coswitch records rs"
    "p in park_sp before it posts the semaphore, every word of the mutator's lives at or above it, and the words below it are glibc's sem_wait frames -- the whole-mapping walk read those raw words a"
    "s cells (the accident test_gate_gc_the_coexpression_roots_are_typed_and_the_parked_stacks_are_segments.sh names) and cost geddump 5.5 s per collection walking 8 MB per parked co-expression");
static char *gc_seg_parked_lo(scrip_coctx_t *c, char *lo) { return (c->park_sp && c->park_sp > lo) ? c->park_sp : lo; }
static int gc_seg_next(gc_seg_it_t *it, char **lo, char **hi, int *pop) {
    if (it->stage == 0) {
        it->stage = 1;
        if (it->co && !pthread_equal(it->self, it->mainthr)) {
            char *slo, *shi;
            gc_stack_region(&slo, &shi);
            if (slo && shi && slo < shi) {
                *lo = gc_seg_parked_lo(scrip_co_gc_root(), slo);
                *hi = shi;
                *pop = 4;
                it->prbp = scrip_co_gc_root()->park_rbp;
                it->sc = scrip_co_gc_root();
#if RT_DIAG
                g_gc_co_parked++;
#endif
                return 1;
            }
        }
    }
    while (it->stage == 1) {
        scrip_coctx_t *c = it->c;
        char *clo, *chi;
        if (!c) { it->stage = 2; break; }
        it->c = c->gc_next;
        if (!scrip_co_stack_of(c, &clo, &chi) || clo >= chi) continue;
        if (c->alive && pthread_equal(it->self, c->thread)) { it->run_hi = chi; it->run_c = c; continue; }
        *lo = gc_seg_parked_lo(c, clo);
        *hi = chi;
        *pop = 4;
        it->prbp = c->park_rbp;
        it->sc = c;
#if RT_DIAG
        g_gc_co_parked++;
#endif
        return 1;
    }
    if (it->stage == 2) {
        it->stage = 3;
        if (it->floor < it->run_hi) {
            char *top = (it->run_hi == g_gc_stktop && g_gc_program_top > it->floor && g_gc_program_top < it->run_hi) ? g_gc_program_top : it->run_hi;
            *lo = it->floor;
            *hi = top;
            *pop = 1;
            it->prbp = (char *)0;
            it->sc = it->run_c;
            g_gc_seg_main = (it->run_hi == g_gc_stktop) ? 1 : 0;
            return 1;
        }
    }
    return 0;
}
static int gc_chain_entry_from_c(uint64_t *pco, const char **ro, const char **rbpo);
static gc_ent_t gc_park_ent(const scrip_coctx_t *sc, const char *lo, const char *prbp);
static long gc_stack_segments(char *floor, const gc_ent_t *ent) {
    gc_seg_it_t it;
    char *lo, *hi;
    int pop;
    long words = 0;
    uint64_t vck[5];
    uint64_t upc = 0;
    const char *ur = (const char *)0, *urbp = (const char *)0;
    gc_ent_t ue;
    int vk;
    if (!ent && gc_chain_entry_from_c(&upc, &ur, &urbp)) { ue.pc = (const void *)(uintptr_t)upc; ue.r = ur; ue.rbp = (const void *)urbp; ent = &ue; }
    vk = ent && gc_frames_visit_check(vck);
    gc_seg_begin(&it, floor);
    while (gc_seg_next(&it, &lo, &hi, &pop)) {
        gc_ent_t pe = gc_park_ent(it.sc, lo, it.prbp);
        int run = g_gc_seg_main && !it.prbp;
#if RT_DIAG
        g_gc_rep_pop = pop;
#endif
        words += gc_visit_segment((const char *)lo, (const char *)hi, it.prbp ? &pe : ent, it.sc, run);
#if RT_DIAG
        g_gc_rep_pop = 0;
#endif
        g_gc_seg_main = 0;
    }
    if (vk) gc_frames_visit_judge(vck);
    return words;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_dvec_ref_ok(const DESCR_t *d) {
    rt_hblk_t *h;
    if (d->v != DT_DATA || d->slen != DATA_ELEMS_SLEN || !d->ptr) return 0;
    h = gc_blk_of((const char *)d->ptr);
    if (!h || h->type != HB_DVEC) return 0;
    return (((const char *)d->ptr - (const char *)(h + 1)) % (long)sizeof(DESCR_t)) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_block_exact(const char *q, uint16_t want_type) {
    if (!q || (uintptr_t)q < sizeof(rt_hblk_t) || ((uintptr_t)q & 7u)) return 0;
    { rt_hblk_t *h = gc_blk_of(q); return (h && (char *)(h + 1) == q && h->type == want_type && (h->flags & HBF_TTL)) ? 1 : 0; }
}
static rt_hblk_t *gc_block_exact_h(const char *q, uint16_t want_type) {
    if (!q || (uintptr_t)q < sizeof(rt_hblk_t) || ((uintptr_t)q & 7u)) return (rt_hblk_t *)0;
    { rt_hblk_t *h = gc_blk_of(q); return (h && (char *)(h + 1) == q && h->type == want_type && (h->flags & HBF_TTL)) ? h : (rt_hblk_t *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_alloc_total(void) { return g_hp_fr.alloc_total; }
long rt_gc_alloc_str(void) { return g_hp_fr.alloc_str; }
long rt_gc_bytes_in_use(void) { return (g_hp_arena && g_hp_top >= g_hp_arena && g_hp_top <= g_hp_cap_end) ? (long)(g_hp_top - g_hp_arena) : g_hp_live; }
#if RT_DIAG
static int g_gc_maps_rep = -1;
#define GC_REP_POPS 5
static const char *const g_gc_rep_popname[GC_REP_POPS] = { "other", "cstack", "seam", "heapblk", "parked" };
static long g_gc_rep_ranges[GC_REP_POPS], g_gc_rep_bytes[GC_REP_POPS], g_gc_rep_agree[GC_REP_POPS], g_gc_rep_map_only[GC_REP_POPS], g_gc_rep_sniff_only[GC_REP_POPS], g_gc_rep_raw[GC_REP_POPS];
static int g_gc_rep_pop = 0;
static int gc_maps_on(void) { if (g_gc_maps_rep < 0) { const char *e = getenv("SCRIP_GC_MAPS"); g_gc_maps_rep = (e && *e && *e != '0') ? 1 : 0; } return g_gc_maps_rep; }
static int gc_maps_verbose(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_MAPS"); v = (e && *e >= '2' && *e <= '9') ? 1 : 0; } return v; }
static FILE *gc_maps_log(void) {
    static FILE *f = (FILE *)0;
    static int tried = 0;
    if (!tried) { const char *e = getenv("SCRIP_GC_MAPS_LOG"); tried = 1; if (e && *e) { f = fopen(e, "a"); if (f) setvbuf(f, (char *)0, _IOLBF, 0); } }
    return f ? f : stderr;
}
static int gc_type_says_ref(const DESCR_t *d) {
    switch (d->v) {
        case DT_S:
        return 1;
        case DT_X:
        return 1;
        case DT_T:
        return 1;
        case DT_A:
        return 1;
        case DT_DATA:
        return 1;
        case DT_BIG:
        return 1;
        case DT_CPLX:
        return 1;
        case DT_EXTL:
        return 1;
        case DT_P:
        return 1;
        case DT_PLVAR:
        return 1;
        case DT_PLREF:
        return 1;
        case DT_E:
        return IS_PROCVAL_fn(*d);
        case DT_N:
        return d->slen == 0 || d->slen == 2;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_sniff_would_take(const DESCR_t *d) {
    if (d->v == DT_S || d->v == DT_X || (d->v == DT_N && d->slen == 0)) {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (h && d->s == (char *)(h + 1) && (d->slen == 0xFFFFFFFFu || (uint64_t)d->slen < (uint64_t)h->size)) return 1;
    }
    if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) return 1; }
    if (d->v == DT_N && d->slen == 2) { rt_hblk_t *vh = gc_blk_of((const char *)d->p); if (vh && vh->type == HB_AGGV && (char *)d->p == (char *)(vh + 1)) return 1; }
    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) return 1;
    if (d->v == DT_E && IS_PROCVAL_fn(*d)) { rt_hblk_t *eh = gc_blk_of(d->s); if (eh && d->s >= (char *)(eh + 1) && d->s < (char *)eh + eh->size) return 1; }
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN) return gc_dvec_ref_ok(d);
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) return 1;
    if (d->v == DT_BIG || d->v == DT_CPLX || d->v == DT_EXTL) { rt_hblk_t *nh = gc_blk_of((const char *)d->p); if (nh && (char *)d->p == (char *)(nh + 1)) return 1; }
    if (d->v == DT_P || d->v == DT_PLVAR || d->v == DT_PLREF) { rt_hblk_t *ph = gc_blk_of((const char *)d->p); if (ph) return 1; }
    return 0;
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_tag_known(uint8_t v);
#if RT_DIAG
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_maps_report_range(const char *lo0, const char *hi0) {
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
typedef struct gc_walk_t {
    long frames, roots, nomap, notab, i_descr, i_heap, i_badtag, i_phase, i_ptr, i_ptr_heap, i_raw, i_raw_heap, i_gap, h_words, h_cell_heap, h_raw_heap, s_words, s_cell_heap, s_raw_heap, a_words,
        a_heap, ghost;
} gc_walk_t;
static gc_walk_t g_gw[GC_REP_POPS];
static long g_gw_lines, g_gw_suppressed;
#define GC_SPINE_REC_CAP 32
typedef struct gc_spine_rec_t { rt_hblk_t *h; const char *graph; long off; const char *const *at; } gc_spine_rec_t;
static gc_spine_rec_t g_gc_spine_rec[GC_SPINE_REC_CAP];
static int g_gc_spine_recn;
static long g_gc_spine_lost, g_gc_spine_seen;
static void gc_spine_record(const char *graph, long off, const char *const *w, rt_hblk_t *h) {
    if (!gc_maps_on()) return;
    g_gc_spine_seen++;
    if (g_gc_spine_recn >= GC_SPINE_REC_CAP) return;
    g_gc_spine_rec[g_gc_spine_recn].h = h;
    g_gc_spine_rec[g_gc_spine_recn].graph = graph;
    g_gc_spine_rec[g_gc_spine_recn].off = off;
    g_gc_spine_rec[g_gc_spine_recn].at = w;
    g_gc_spine_recn++;
}
#endif
static int gc_tag_known(uint8_t v) { return v == DT_SNUL || v == DT_S || v == DT_I || v == DT_R || ((v & 7u) == 0 && v >= DT_P && v <= DT_RAW) || v == DT_PLATOM; }
#if RT_DIAG
static void gc_walk_site(const char *cls, const char *graph, long off, const char *const *w, rt_hblk_t *h) {
    if (!gc_maps_on()) return;
    if (g_gw_lines >= GC_WALK_LINE_CAP) { g_gw_suppressed++; return; }
    g_gw_lines++;
    {
        const unsigned char *b = (const unsigned char *)(h + 1);
        long n = (long)h->size - (long)sizeof(rt_hblk_t), m = n < 24 ? (n > 0 ? n : 0) : 24;
        char tx[m + 1];
        int i = 0;
        for (; i < m; i++) tx[i] = (b[i] >= 32 && b[i] < 127) ? (char)b[i] : '.';
        tx[i] = 0;
        fprintf(stderr, "[GC-WALK-%s] pop=%s graph=%s off=%ld at=%p word=%p blk=%p type=%u text=%s\n", cls, g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", off, (const void *)w, (const void *)*w,
            (const void *)h, (unsigned)h->type, tx);
    }
}
static void gc_walk_badtag(const char *graph, long off, const DESCR_t *d) {
    if (!gc_maps_on()) return;
    if (g_gw_lines >= GC_WALK_LINE_CAP) { g_gw_suppressed++; return; }
    g_gw_lines++;
    fprintf(stderr, "[GC-WALK-BADTAG] pop=%s graph=%s off=%ld v=0x%02x slen=%u p=%p\n", g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", off, (unsigned)d->v, (unsigned)d->slen, (const void *)d->p)
        ;
}
static int gc_maps_dump(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_MAPS"); v = (e && *e >= '3' && *e <= '9') ? 1 : 0; } return v; }
#define GC_WALK_DUMP_CAP 6
static long g_gw_dumps;
static int gc_code_range(const char *w) {
    static unsigned long *xr;
    static int n = -1;
    if (n < 0) {
        FILE *f = fopen("/proc/self/maps", "r");
        n = 0;
        if (f) {
            int r, st;
            unsigned long a, b;
            char pm[8];
            while ((r = gc_maps_line(f, &a, &b, pm, &st)) >= 0) if (r && pm[1] == '-' && pm[2] == 'x') {
                xr = (unsigned long *)gcbk_reserve((void *)xr, (size_t)(n + 1) * 2 * sizeof(unsigned long));
                xr[2 * n] = a;
                xr[2 * n + 1] = b;
                n++;
            }
            fclose(f);
        }
    }
    for (int i = 0; i < n; i++) if ((unsigned long)w >= xr[2 * i] && (unsigned long)w < xr[2 * i + 1]) return 1;
    return 0;
}
static void gc_walk_dump(const char *lo, const char *base, const char *graph) {
    if (!gc_maps_dump() || g_gw_dumps >= GC_WALK_DUMP_CAP || base - lo <= 64) return;
    g_gw_dumps++;
    fprintf(stderr, "[GC-WALK-DUMP] pop=%s graph=%s floor=%p base=%p words=%ld\n", g_gc_rep_popname[g_gc_rep_pop], graph ? graph : "-", (const void *)lo, (const void *)base, (long)(base - lo) / 8);
    for (const char *p = lo; p + 8 <= base; p += 8) {
        const char *w = *(const char *const *)p;
        rt_hblk_t *h = gc_blk_of(w);
        const char *cls = "int";
        unsigned ty = 0;
        int cell = (p - 8 >= lo) && gc_tag_bears_ptr(*(const uint8_t *)(p - 8));
        if (h) { cls = cell ? "heap-cell" : "heap-RAW"; ty = h->type; } else if (!w) cls = "nil";
        else if (gc_code_range(w)) {
            Dl_info di;
            cls = "code";
            if (dladdr((const void *)w, &di) && di.dli_sname) {
                fprintf(stderr, "[GC-WALK-DUMP] off=%ld at=%p word=%p cls=code sym=%s+%ld\n", (long)(p - base), (const void *)p, (const void *)w, di.dli_sname, (long)(w - (const char *)di.dli_saddr));
                continue;
            }
        } else if (w >= lo - (1L << 20) && w <= gc_stack_top()) cls = "stack";
        else if (((uintptr_t)w & 0xFFFFFF00u) == 0 && gc_tag_known((uint8_t)(uintptr_t)w)) cls = "tag";
        else if (((uintptr_t)w & 0xFFFFFF00u) == 0 && gc_tag_known((uint8_t)(uintptr_t)w) && ((uintptr_t)w >> 32) != 0) cls = "tag";
        fprintf(stderr, "[GC-WALK-DUMP] off=%ld at=%p word=%p cls=%s type=%u\n", (long)(p - base), (const void *)p, (const void *)w, cls, ty);
    }
}
#endif
static int gc_cell_visit(DESCR_t *d) {
    if (d->v == DT_S || d->v == DT_X || (d->v == DT_N && d->slen == 0)) {
        rt_hblk_t *h = gc_blk_of(d->s);
        if (h && d->s >= (char *)(h + 1) && d->s < (char *)h + h->size) { rt_gc_visit_descr(d); return 1; }
        return 0;
    }
    if (d->v == DT_T) { rt_hblk_t *th = gc_blk_of((const char *)d->tbl); if (th && th->type == HB_AGGT && (char *)d->tbl == (char *)(th + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_N && d->slen == 2) {
        rt_hblk_t *vh = gc_blk_of((const char *)d->p);
        if (vh && vh->type == HB_AGGV && (char *)d->p == (char *)(vh + 1)) { rt_gc_visit_descr(d); return 1; }
        return 0;
    }
    if (d->v == DT_N && d->slen != 0 && d->slen != 1 && d->slen != 2) { if (gc_blk_of(d->s)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_N && d->slen == 1) {
        rt_hblk_t *ch = gc_blk_of((const char *)d->ptr);
        if (ch && (char *)d->ptr >= (char *)(ch + 1) && (char *)d->ptr + 16 <= (char *)ch + ch->size) { rt_gc_visit_descr(d); return 1; }
        return 0;
    }
    if (d->v == DT_A && gc_block_exact((const char *)d->arr, HB_ARR)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_E && IS_PROCVAL_fn(*d)) { rt_hblk_t *eh = gc_blk_of(d->s); if (eh && d->s >= (char *)(eh + 1) && d->s < (char *)eh + eh->size) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (gc_dvec_ref_ok(d)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_INST_SLEN && gc_block_exact((const char *)d->u, HB_DINST)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_DATA && d->slen == DATA_ELEMS_SLEN && gc_block_exact((const char *)d->ptr, HB_DVEC)) { rt_gc_visit_descr(d); return 1; }
    if (d->v == DT_P || d->v == DT_PLVAR || d->v == DT_PLREF) { if (gc_blk_of((const char *)d->p)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_BIG) { rt_hblk_t *bh = gc_blk_of((const char *)d->p); if (bh && bh->type == HB_WSB && (char *)d->p == (char *)(bh + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_CPLX) { rt_hblk_t *ch = gc_blk_of((const char *)d->p); if (ch && ch->type == HB_WSB && (char *)d->p == (char *)(ch + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    if (d->v == DT_EXTL) { rt_hblk_t *xh = gc_blk_of((const char *)d->p); if (xh && xh->type == HB_WSB && (char *)d->p == (char *)(xh + 1)) { rt_gc_visit_descr(d); return 1; } return 0; }
    return 0;
}
#if RT_DIAG
static int gc_nospine_cell(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_NO_SPINE_CELL"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int gc_walk_census_on(void) { return gc_diag_on() || gc_maps_on() || gc_nospine_cell(); }
#endif
#if RT_DIAG
static void gc_walk_words_census(const char *lo, const char *hi, int cls, const char *rlo, const char *graph, const char *base) {
    gc_walk_t *g = &g_gw[g_gc_rep_pop];
    for (const char *p = lo; p + 8 <= hi; p += 8) {
        const char **w = (const char **)p;
        rt_hblk_t *h = gc_blk_of(*w);
        if (cls == 0) g->s_words++;
        else if (cls == 1) g->h_words++;
        else g->a_words++;
        if (cls != 2 && !(cls == 0 && gc_nospine_cell()) && p + 16 <= hi && gc_cell_visit((DESCR_t *)p)) {
            if (gc_blk_of(*(const char *const *)(p + 8))) { if (cls == 0) g->s_cell_heap++; else g->h_cell_heap++; }
            p += 8;
            if (cls == 0) g->s_words++;
            else g->h_words++;
            continue;
        }
        if (!h) continue;
        if (cls == 2) { g->a_heap++; gc_walk_site("ABOVE", graph, (long)(p - base), w, h); continue; }
        if (p - 8 >= rlo && gc_tag_bears_ptr(*(const uint8_t *)(p - 8))) { if (cls == 0) g->s_cell_heap++; else g->h_cell_heap++; continue; }
        if (cls == 0) {
            g->s_raw_heap++;
            gc_walk_site("SPINE", graph, (long)(p - base), w, h);
            gc_spine_record(graph, (long)(p - base), w, h);
        } else {
            g->h_raw_heap++;
            gc_walk_site("HEADER", graph, (long)(p - base), w, h);
        }
    }
}
#endif
static void gc_walk_words(const char *lo, const char *hi, int cls, const char *rlo, const char *graph, const char *base) {
    extern DESCR_t *rt_gc_walk_words_a(const char *, const char *);
    (void)rlo;
    (void)graph;
    (void)base;
    if (cls == 2 || lo + 16 > hi) return;
    gc_run_verdicts(rt_gc_walk_words_a(lo, hi));
}
#if RT_DIAG
static long g_gc_blob_frames, g_gc_blob_entries, g_gc_blob_descr, g_gc_blob_ptr, g_gc_blob_raw, g_gc_blob_code, g_gc_blob_raw_in_heap;
#endif
#if RT_DIAG
static void gc_visit_frame_cells_census(const char *anchor, const gc_frame_map_t *m, const char *lo, const char *hi) {
    gc_walk_t *g = &g_gw[g_gc_rep_pop];
    const uint64_t *t = (const uint64_t *)(m + 1);
    long n = (long)t[0];
    long covered = 0;
    int blob = (m->flags & GC_FRAME_MAP_BLOB) ? 1 : 0;
    if (blob) g_gc_blob_frames++;
    long span = (m->flags & GC_FRAME_MAP_BLOB) ? (long)m->frame_bytes + 8 : (long)m->value_bytes;
    for (long i = 0; i < n; i++) {
        uint64_t q = t[1 + i];
        int off = GC_LAY_OFF(q), size = GC_LAY_SIZE(q);
        unsigned kind = GC_LAY_KIND(q);
        const char *w = anchor + off, *e = w + size;
        if (w < lo) w = (kind == GC_LAY_DESCR) ? w + (((lo - w) + 15) & ~15L) : lo;
        if (e > hi) e = hi;
        if (w >= e) continue;
        covered += e - w;
        if (blob) {
            g_gc_blob_entries++;
            if (kind == GC_LAY_DESCR) g_gc_blob_descr++;
            else if (kind == GC_LAY_PTR_GC) g_gc_blob_ptr++;
            else if (kind == GC_LAY_RAW) g_gc_blob_raw++;
            else g_gc_blob_code++;
        }
        if (kind == GC_LAY_DESCR) {
            for (const char *c = w; c + 16 <= e; c += 16) {
                DESCR_t *d = (DESCR_t *)c;
                g->i_descr++;
                if (!gc_tag_known(d->v)) {
                    g->i_badtag++;
                    if (gc_tag_known((uint8_t)(uintptr_t)d->p)) g->i_phase++;
                    gc_walk_badtag(m->graph_name, (long)(c - anchor), d);
                } else {
                    if (gc_tag_bears_ptr(d->v) && gc_blk_of((const char *)d->p)) g->i_heap++;
                    rt_gc_visit_descr(d);
                }
            }
            continue;
        }
        for (const char *c = w; c + 8 <= e; c += 8) {
            const char **pw = (const char **)c;
            rt_hblk_t *h = gc_blk_of(*pw);
            if (kind == GC_LAY_PTR_GC) { g->i_ptr++; if (h) { g->i_ptr_heap++; rt_gc_visit_raw(pw); } continue; }
            g->i_raw++;
            if (h) {
                g->i_raw_heap++;
                gc_walk_site("RAW", m->graph_name, (long)(c - anchor), pw, h);
                if (blob && gc_type_moves(h->type)) {
                    g_gc_blob_raw_in_heap++;
                    if (gc_maps_on())
                        fprintf(stderr, "[GC-BLOB-RAW] graph=%s off=%ld kind=%u word=%p blk=%p type=%u\n", m->graph_name ? m->graph_name : "?", (long)(c - anchor), kind, (const void *)*pw,
                        (const void *)h, (unsigned)h->type);
                }
            }
        }
    }
    if (span > covered) g->i_gap += (span - covered) / 8;
}
#endif
static void gc_visit_frame_cells(const char *anchor, const gc_frame_map_t *m, const char *lo, const char *hi) {
    extern DESCR_t *rt_gc_frame_cells_a(const char *, const gc_frame_map_t *, const char *, const char *);
    gc_run_verdicts(rt_gc_frame_cells_a(anchor, m, lo, hi));
}
static void gc_walk_words_x(const char *lo, const char *hi, int cls, const char *rlo, const char *graph, const char *base) {
#if RT_DIAG
    if (g_gc_mk.census) { gc_walk_words_census(lo, hi, cls, rlo, graph, base); return; }
#endif
    gc_walk_words(lo, hi, cls, rlo, graph, base);
}
static void gc_visit_frame_cells_x(const char *anchor, const gc_frame_map_t *m, const char *lo, const char *hi) {
#if RT_DIAG
    if (g_gc_mk.census) { gc_visit_frame_cells_census(anchor, m, lo, hi); return; }
#endif
    gc_visit_frame_cells(anchor, m, lo, hi);
}
typedef struct { const char *base; const gc_frame_map_t *map; uint64_t link; int rule, ext; } gc_chf_t;
typedef struct { int st; uint64_t pc; const gc_site_ent_t *e; const gc_site_blk_t *b; const char *base, *r, *rbp; } gc_chx_t;
static struct {
    long runs, whole, frames, offchain, said, missing, mapdiff, xblob, xgen, xroot, xplain, beyond, shrt, s_root, s_nosite, s_frameless, s_unknown, s_cycle, s_nolink, s_rise, s_left, s_costart,
        s_badlink, s_noentry, s_topabove, links, links_below, said_link, v_same, v_diff, v_refused, ghost;
    uint64_t fsum;
} g_gc_chw;
static int g_gc_chain_knob = -1, g_gc_chain_reg = 0;
static long g_gc_chain_said_fl = 0, g_gc_chain_said_dump = 0, g_gc_chain_said_ctx = 0;
static int gc_chain_check_on(void) {
    if (g_gc_chain_knob < 0) { const char *e = getenv("SCRIP_GC_CHAIN_CHECK"); g_gc_chain_knob = (e && *e >= '1' && *e <= '3') ? (*e - '0') : 0; }
    return g_gc_chain_knob;
}
static long g_gc_chain_ok = 0, g_gc_chain_nosite = 0, g_gc_chain_host = 0, g_gc_chain_bad = 0, g_gc_chain_said = 0, g_gc_chain_frameless = 0, g_gc_chain_ctx = 0, g_gc_chain_fnhops = 0,
    g_gc_chain_landhops = 0, g_gc_chain_roadhops = 0, g_gc_chain_ghost = 0;
extern char rt_tge_gamma_ret[], rt_tge_omega_ret[];
extern const uint64_t rt_genp_n2_conts[2];
extern const uint64_t rt_genp_conts[3];
static void gc_chain_report_atexit(void) {
    fprintf(stderr, "[CHAIN-CHECK] SUMMARY ok=%ld host=%ld frameless=%ld ctxrbp=%ld nosite=%ld mismatch=%ld fnhops=%ld landhops=%ld roadhops=%ld ghost=%ld\n", g_gc_chain_ok, g_gc_chain_host,
        g_gc_chain_frameless, g_gc_chain_ctx, g_gc_chain_nosite, g_gc_chain_bad, g_gc_chain_fnhops, g_gc_chain_landhops, g_gc_chain_roadhops, g_gc_chain_ghost);
}
static const gc_site_blk_t *gc_root_block(void) {
    const gc_frame_map_t *r = gc_root_map();
    for (int i = 0; r && i < g_gc_sblk_n; i++) if (g_gc_sblk[i].map == r) return &g_gc_sblk[i];
    return (const gc_site_blk_t *)0;
}
static void gc_chain_find(gc_chx_t *cx, uint64_t pc, const char *r, const char *rbp, const char *seg_top) {
    const gc_site_ent_t *e = (const gc_site_ent_t *)0;
    const gc_site_blk_t *b;
    const char *base;
    uint64_t cpc = 0;
    const char *cr = (const char *)0, *crbp = (const char *)0;
    long lam = 0, pw = 1;
    cx->st = 0;
    if (!r) return;
    again:
    if (pc == cpc && r == cr && rbp == crbp) { cx->pc = pc; cx->r = r; cx->rbp = rbp; cx->st = 5; return; }
    if (++lam == pw) { pw *= 2; lam = 0; cpc = pc; cr = r; crbp = rbp; }
    if (pc == (uint64_t)(uintptr_t)rt_tge_gamma_ret || pc == (uint64_t)(uintptr_t)rt_tge_omega_ret) {
        uint64_t c = *(const uint64_t *)r;
        const char *r2 = r + ((39 + 8 * c) & ~(uint64_t)15) + 16 * c;
        pc = *(const uint64_t *)(r2 + (pc == (uint64_t)(uintptr_t)rt_tge_gamma_ret ? 8 : 16));
        r = r2;
        g_gc_chain_roadhops++;
        goto again;
    }
    if (pc == rt_genp_n2_conts[0] || pc == rt_genp_n2_conts[1] || pc == rt_genp_conts[0] || pc == rt_genp_conts[1] || pc == rt_genp_conts[2]) {
        cx->pc = pc;
        cx->r = r;
        cx->rbp = rbp;
        cx->st = 7;
        return;
    }
    b = gc_site_find(pc, &e);
    if (b && e && (e->kind & 255) == 7) { pc = *(const uint64_t *)(r + e->depth); r = r + e->depth + 8; g_gc_chain_roadhops++; goto again; }
    if (!b || !e) {
        extern char rt_genp_n2_deliver_ret[];
        const char *fp = rbp, *top = seg_top ? seg_top : g_gc_stktop;
        while (fp && fp > r && fp + 16 <= top) {
            uint64_t pc2 = *(const uint64_t *)(fp + 8);
            const char *fp2 = *(const char *const *)fp;
            if (pc2 == (uint64_t)(uintptr_t)rt_genp_n2_deliver_ret) { cx->pc = pc2; cx->r = fp + 16; cx->rbp = fp2; cx->st = 6; return; }
            b = gc_site_find(pc2, &e);
            if (b && e) { pc = pc2; r = fp + 16; rbp = fp2; break; }
            if (fp2 <= fp) break;
            fp = fp2;
        }
    }
    cx->pc = pc;
    cx->r = r;
    cx->rbp = rbp;
    if (!b || !e) { cx->st = 2; return; }
    cx->e = e;
    cx->b = b;
    if (GC_SITE_RULE(e) == 9) {
        const char *fb = r + 8 * ((e->kind >> 8) & 255);
        int loff = (int)((uint32_t)e->depth & 0xffffu), carve = (int)(((uint32_t)e->depth >> 16) & 0xffffu);
        const char *L = *(const char *const *)(fb + loff);
        pc = *(const uint64_t *)(L + 8);
        r = fb + 16 + carve;
        g_gc_chain_fnhops++;
        goto again;
    }
    if (!b->map) {
        const char *fbm = (const char *)0, *rbm = rbp;
        if (GC_SITE_RULE(e) == 0) fbm = r + e->depth;
        else if (GC_SITE_RULE(e) == 6 && ((e->kind >> 12) & 15) == 0) {
            const char *fp = rbp;
            for (int k = 1; k < ((e->kind >> 8) & 15); k++) fp = *(const char *const *)fp;
            fbm = fp + 8 + e->depth;
            rbm = *(const char *const *)fp;
        }
        if (fbm) {
            uint64_t w = b->ckt >= 48 ? 0 : *(const uint64_t *)fbm;
            const gc_site_ent_t *x = (const gc_site_ent_t *)0;
            const gc_site_blk_t *xb = gc_site_find(w, &x);
            if (xb && x && (x->kind & 255) == 6) {
                int loff = (int)(x->depth & 0xffff), carve = (int)((x->depth >> 16) & 0xffff);
                const char *L = *(const char *const *)(fbm + loff);
                pc = *(const uint64_t *)(L + 8);
                r = (GC_SITE_RULE(x) == 8) ? L : fbm + 16 + carve;
                rbp = rbm;
                g_gc_chain_fnhops++;
                goto again;
            }
            if (xb && x && (x->kind & 255) == 2) { pc = w; r = fbm; rbp = rbm; g_gc_chain_landhops++; goto again; }
            if (b->ckt >= 48) {
                uint64_t g = *(const uint64_t *)(fbm + b->ckt - 24);
                const gc_site_ent_t *x2 = (const gc_site_ent_t *)0;
                const gc_site_blk_t *xb2 = gc_site_find(g, &x2);
                if (xb2 && x2 && (x2->kind & 255) == 2) { pc = g; r = fbm + b->ckt; rbp = rbm; g_gc_chain_landhops++; goto again; }
                if (xb2 && x2 && (x2->kind & 255) == 6) {
                    const char *a0 = fbm + b->ckt;
                    int loff = (int)(x2->depth & 0xffff), carve = (int)((x2->depth >> 16) & 0xffff);
                    const char *lrec = *(const char *const *)(a0 + loff);
                    pc = *(const uint64_t *)(lrec + 8);
                    r = (GC_SITE_RULE(x2) == 8) ? lrec : a0 + 16 + carve;
                    rbp = rbm;
                    g_gc_chain_fnhops++;
                    goto again;
                }
                if (fbm + b->ckt + 16 <= seg_top) {
                    const char *a0 = fbm + b->ckt;
                    uint64_t tw = *(const uint64_t *)(a0 + 8);
                    if (g == (uint64_t)(uintptr_t)a0 && tw == (uint64_t)(uintptr_t)rt_tge_gamma_ret) { pc = tw; r = a0; rbp = rbm; g_gc_chain_roadhops++; goto again; }
                    const gc_site_ent_t *x3 = (const gc_site_ent_t *)0;
                    const gc_site_blk_t *xb3 = gc_site_find(*(const uint64_t *)a0, &x3);
                    if (xb3 && x3 && (x3->kind & 255) == 6) {
                        int loff = (int)(x3->depth & 0xffff), carve = (int)((x3->depth >> 16) & 0xffff);
                        const char *lrec = *(const char *const *)(a0 + loff);
                        pc = *(const uint64_t *)(lrec + 8);
                        r = (GC_SITE_RULE(x3) == 8) ? lrec : a0 + 16 + carve;
                        rbp = rbm;
                        g_gc_chain_fnhops++;
                        goto again;
                    }
                }
                if (g > 4095) {
                    uint64_t sg = *(const uint64_t *)(uintptr_t)(g + 8);
                    const gc_site_ent_t *y = (const gc_site_ent_t *)0;
                    const gc_site_blk_t *yb = gc_site_find(sg, &y);
                    if (yb && y && (y->kind & 255) == 2) { pc = sg; r = fbm + b->ckt; rbp = rbm; g_gc_chain_landhops++; goto again; }
                }
            }
            if (b->ckt > 0 && g_gc_program_top) {
                const gc_site_blk_t *rb = gc_root_block();
                if (rb && fbm + b->ckt + (long)rb->map->value_bytes + 16 + (long)rb->map->header_bytes == g_gc_program_top) { cx->b = rb; cx->base = fbm + b->ckt; cx->st = 1; return; }
            }
        }
    }
    if (!b->map) { cx->st = 3; return; }
    const char *rbm6 = (const char *)0;
    if (GC_SITE_RULE(e) == 6) {
        int nl = (e->kind >> 8) & 15, ek = (e->kind >> 12) & 15;
        const char *fp = rbp;
        for (int k = 1; k < nl; k++) fp = *(const char *const *)fp;
        base = (ek == 1 || ek == 3) ? *(const char *const *)fp : fp + 8 + e->depth;
        if (ek == 0) rbm6 = *(const char *const *)fp;
    } else if (GC_SITE_RULE(e) == 0) base = r + e->depth;
    else if (GC_SITE_RULE(e) == 1) base = rbp;
    else if (GC_SITE_RULE(e) == 2) base = rbp - e->depth;
    else if (GC_SITE_RULE(e) == 3) base = rbp;
    else if (GC_SITE_RULE(e) == 4) base = *(const char *const *)r + e->depth;
    else base = (const char *)0;
    if (GC_SITE_RULE(e) == 0 || rbm6) {
        uint64_t w = *(const uint64_t *)base;
        const gc_site_ent_t *x = (const gc_site_ent_t *)0;
        const gc_site_blk_t *xb = gc_site_find(w, &x);
        if (xb && x && (x->kind & 255) == 6) {
            int loff = (int)(x->depth & 0xffff), carve = (int)((x->depth >> 16) & 0xffff);
            const char *L = *(const char *const *)(base + loff);
            pc = *(const uint64_t *)(L + 8);
            r = (GC_SITE_RULE(x) == 8) ? L : base + 16 + carve;
            if (rbm6) rbp = rbm6;
            g_gc_chain_fnhops++;
            goto again;
        }
        if (xb && x && (x->kind & 255) == 2) { pc = w; r = base; if (rbm6) rbp = rbm6; g_gc_chain_landhops++; goto again; }
    }
    cx->base = base;
    cx->st = (GC_SITE_RULE(e) == 5) ? 4 : 1;
}
static void gc_chain_resolve_in(gc_chx_t *cx, const gc_ent_t *ent, const char *seg_top) {
    memset(cx, 0, sizeof *cx);
    if (ent) gc_chain_find(cx, (uint64_t)(uintptr_t)ent->pc, ent->r, (const char *)ent->rbp, seg_top);
}
static void gc_chain_resolve(gc_chx_t *cx, const gc_ent_t *ent) { gc_chain_resolve_in(cx, ent, (const char *)0); }
const char *rt_icn_map_proc_name(const gc_frame_map_t *m) { const char *g = m ? m->graph_name : (const char *)0; if (!g) return "?"; return strncmp(g, "proc_", 5) == 0 ? g + 5 : g; }
long rt_icn_map_args_off(const gc_frame_map_t *m) {
    if (!m) return 0;
    if (m->flags & GC_FRAME_MAP_GEN_ANCHOR) return (long)(((m->frame_bytes + 15u) & ~15u) + 72u);
    return (m->flags & GC_FRAME_MAP_ICN_BLOCK) ? (long)m->frame_bytes : 16L;
}
typedef struct { uint64_t pc; const char *r, *rbp; int hit; } gc_uw_t;
static _Unwind_Reason_Code gc_uw_step(struct _Unwind_Context *c, void *a) {
    gc_uw_t *u = (gc_uw_t *)a;
    const gc_site_ent_t *e = (const gc_site_ent_t *)0;
    u->pc = (uint64_t)_Unwind_GetIP(c);
    u->r = (const char *)_Unwind_GetCFA(c);
    u->rbp = (const char *)_Unwind_GetGR(c, 6);
    if (u->pc && gc_site_find(u->pc, &e) && e) { u->hit = 1; return _URC_END_OF_STACK; }
    return _URC_NO_REASON;
}
static int gc_chain_entry_core(uint64_t *pco, const char **ro, const char **rbpo, int strict) {
    const char *top = g_gc_stktop, *fp;
    gc_uw_t u = { 0, (const char *)0, (const char *)0, 0 };
    _Unwind_Backtrace(gc_uw_step, &u);
    if (u.hit) { *pco = u.pc; *ro = u.r; *rbpo = u.rbp; return 1; }
    fp = u.rbp;
    while (strict && fp && fp + 16 <= top) {
        uint64_t ra = *(const uint64_t *)(fp + 8);
        const char *fp2 = *(const char *const *)fp;
        const gc_site_ent_t *e = (const gc_site_ent_t *)0;
        if (gc_site_find(ra, &e) && e) rt_bomb("gc_chain_entry_from_c: the unwinder missed an emitted frame the rbp chain reaches (an asm leaf without true CFI, or an emitted pc with no site)");
        if (!fp2 || fp2 <= fp) break;
        fp = fp2;
    }
    return 0;
}
static int gc_chain_entry_from_c(uint64_t *pco, const char **ro, const char **rbpo) { return gc_chain_entry_core(pco, ro, rbpo, 1); }
void rt_gc_park_site(uint64_t *pc, char **r, char **rbp) {
    const char *cr = (const char *)0, *crbp = (const char *)0;
    uint64_t cpc = 0;
    if (!gc_chain_entry_core(&cpc, &cr, &crbp, 0)) cpc = 0;
    *pc = cpc;
    *r = (char *)(uintptr_t)cr;
    *rbp = (char *)(uintptr_t)crbp;
}
void rt_gc_park_site_at(uint64_t *pc, char **r, char **rbp, uint64_t epc, char *er, char *erbp) {
    const gc_site_ent_t *e = (const gc_site_ent_t *)0;
    if (epc && gc_site_find(epc, &e) && e) { *pc = epc; *r = er; *rbp = erbp; return; }
    rt_gc_park_site(pc, r, rbp);
}
static gc_ent_t gc_park_ent(const scrip_coctx_t *sc, const char *lo, const char *prbp) {
    if (sc && sc->park_pc) return (gc_ent_t){ (const void *)(uintptr_t)sc->park_pc, sc->park_r, sc->park_srbp };
    return (gc_ent_t){ (const void *)0, lo, prbp };
}
int rt_gc_caller_frame(const void **map, const char **base) {
    uint64_t pc = 0;
    const char *r = (const char *)0, *rbp = (const char *)0;
    gc_chx_t cx;
    if (!gc_chain_entry_from_c(&pc, &r, &rbp)) return 0;
    { gc_ent_t ent = { (const void *)(uintptr_t)pc, r, (const void *)rbp }; gc_chain_resolve(&cx, &ent); }
    if (cx.st != 1 || !cx.b || !cx.b->map || !cx.base) return 0;
    *map = (const void *)cx.b->map;
    *base = cx.base;
    return 1;
}
int rt_icn_frames(rt_icn_frame_t *out, int cap) {
    extern int rt_proc_nparams(const char *);
    const char *r = (const char *)0, *rbp = (const char *)0;
    uint64_t pc = 0;
    int n = 0;
    const char *prev = (const char *)0;
    if (!gc_chain_entry_from_c(&pc, &r, &rbp)) return 0;
    for (; pc && r;) {
        gc_chx_t cx;
        gc_ent_t ent = { (const void *)(uintptr_t)pc, r, (const void *)rbp };
        const gc_frame_map_t *m;
        const char *base;
        gc_chain_resolve(&cx, &ent);
        if (cx.st != 1 || !cx.b || !cx.b->map || !cx.base) break;
        m = cx.b->map;
        base = cx.base;
        if (prev && base <= prev) break;
        prev = base;
        if (!(m->flags & GC_FRAME_MAP_ICN_PROC)) break;
        if (m->flags & GC_FRAME_MAP_GEN_ANCHOR) {
            const char *h = base + m->frame_bytes;
            if (out && n < cap) { out[n].map = m; out[n].base = base; out[n].caller_pc = *(const uint64_t *)(h + 8); }
            n++;
            if (m->flags & GC_FRAME_MAP_ROOT) break;
            pc = *(const uint64_t *)(h + 8);
            rbp = *(const char *const *)h;
            r = *(const char *const *)(h + 24);
        } else {
            int np = rt_proc_nparams(rt_icn_map_proc_name(m));
            const char *a = base + m->frame_bytes + 16L * (np > 0 ? np : 0);
            uint64_t cpc = (m->flags & GC_FRAME_MAP_ICN_BLOCK) ? *(const uint64_t *)a : 0;
            if (out && n < cap) { out[n].map = m; out[n].base = base; out[n].caller_pc = cpc; }
            n++;
            if ((m->flags & GC_FRAME_MAP_ROOT) || !(m->flags & GC_FRAME_MAP_ICN_BLOCK)) break;
            if (m->flags & GC_FRAME_MAP_ICN_PINNED) rbp = *(const char *const *)(base + m->frame_bytes - 8);
            pc = cpc;
            r = a;
        }
    }
    return n;
}
static int gc_chain_link(const char *base, uint64_t q, uint64_t *pc, const char **r, const char **rbp) {
    if (!GC_LINK_SET(q)) return 0;
    *pc = *(const uint64_t *)(base + GC_LINK_PC(q));
    if (!(GC_LINK_FL(q) & GC_LINK_RBP_KEPT)) *rbp = *(const char *const *)(base + GC_LINK_RBP(q));
    *r = (GC_LINK_FL(q) & GC_LINK_R_IND) ? *(const char *const *)(base + GC_LINK_R(q)) : base + GC_LINK_R(q);
    return 1;
}
static const gc_site_blk_t *gc_body_block(const scrip_coctx_t *sc) {
    const gc_site_blk_t *hit = (const gc_site_blk_t *)0;
    uint64_t pc, best = 0;
    if (!sc || !sc->gen_h) return hit;
    pc = *(const uint64_t *)(sc->gen_h + 32);
    for (int i = 0; i < g_gc_sblk_n; i++) if (g_gc_sblk[i].code && g_gc_sblk[i].code <= pc && g_gc_sblk[i].code > best) best = g_gc_sblk[i].code;
    for (int i = 0; best && i < g_gc_sblk_n; i++) {
        const gc_site_blk_t *k = &g_gc_sblk[i];
        if (k->code != best || !k->map || !(k->map->flags & GC_FRAME_MAP_GEN_ANCHOR)) continue;
        if (hit && hit->map != k->map) rt_bomb("gc_body_block: two generator maps share one graph's code start -- the suspended body's frame shape is not named");
        hit = k;
    }
    return hit;
}
static const gc_site_blk_t *gc_code_block(uint64_t pc) {
    const gc_site_blk_t *hit = (const gc_site_blk_t *)0;
    uint64_t best = 0;
    if (!g_gc_mk.code_unsorted) {
        long a = 0, z = (long)g_gc_sblk_n - 1, b = -1;
        while (a <= z) { long mid = (a + z) >> 1; if (g_gc_sblk[mid].lo <= pc) { b = mid; a = mid + 1; } else z = mid - 1; }
        for (; b >= 0 && !(g_gc_sblk[b].map && g_gc_sblk[b].code); b--) ;
        if (b < 0) return (const gc_site_blk_t *)0;
        hit = &g_gc_sblk[b];
        best = hit->code;
        for (long i = b - 1; i >= 0 && g_gc_sblk[i].lo == best; i--) if (g_gc_sblk[i].map && g_gc_sblk[i].code == best && g_gc_sblk[i].map != hit->map) return (const gc_site_blk_t *)0;
        return hit;
    }
    for (int i = 0; i < g_gc_sblk_n; i++) if (g_gc_sblk[i].map && g_gc_sblk[i].code && g_gc_sblk[i].code <= pc && g_gc_sblk[i].code > best) best = g_gc_sblk[i].code;
    for (int i = 0; best && i < g_gc_sblk_n; i++) {
        const gc_site_blk_t *k = &g_gc_sblk[i];
        if (k->code != best || !k->map) continue;
        if (hit && hit->map != k->map) return (const gc_site_blk_t *)0;
        hit = k;
    }
    return hit;
}
static int gc_chf_cmp(const void *x, const void *y) { const char *a = ((const gc_chf_t *)x)->base, *b = ((const gc_chf_t *)y)->base; return a < b ? -1 : a > b ? 1 : 0; }
static long gc_sw_segment_fn(const char *lo0, const char *hi0, const gc_ent_t *ent, const scrip_coctx_t *sc, int run, rt_gc_frame_fn fn, void *a);
int rt_gc_frames_visit(rt_gc_frame_fn fn, void *a) {
    gc_seg_it_t it;
    char *lo, *hi, floor;
    int pop, pass, ok = 1, saved = g_gc_seg_main;
    uint64_t pc = 0;
    const char *r = (const char *)0, *rbp = (const char *)0;
    int entered = gc_chain_entry_from_c(&pc, &r, &rbp);
    gc_ent_t ce = { (const void *)(uintptr_t)pc, r, (const void *)rbp };
#if RT_DIAG
    long park0 = g_gc_co_parked;
#endif
    for (pass = 0; ok && pass < 2; pass++) {
        gc_seg_begin(&it, &floor);
        while (ok && gc_seg_next(&it, &lo, &hi, &pop)) {
            gc_ent_t pe = gc_park_ent(it.sc, lo, it.prbp);
            const gc_ent_t *ent = it.prbp ? &pe : entered ? &ce : (const gc_ent_t *)0;
            int run = g_gc_seg_main && !it.prbp;
            g_gc_seg_main = 0;
            if (gc_sw_segment_fn(lo, hi, ent, it.sc, run, pass ? fn : (rt_gc_frame_fn)0, a) < 0) ok = 0;
        }
    }
#if RT_DIAG
    g_gc_co_parked = park0;
#endif
    g_gc_seg_main = saved;
    return ok;
}
static void gc_frames_visit_sum(const void *map, const char *base, void *a) { uint64_t *s = (uint64_t *)a; s[0]++; s[1] = s[1] * 31u + ((uint64_t)(uintptr_t)base ^ (uint64_t)(uintptr_t)map); }
static int gc_frames_visit_check(uint64_t *v) {
    if (g_gc_mk.chain != 3) return 0;
    v[0] = v[1] = 0;
    v[2] = (uint64_t)g_gc_chw.frames;
    g_gc_chw.fsum = 0;
    v[4] = (uint64_t)rt_gc_frames_visit(gc_frames_visit_sum, v);
    return 1;
}
static void gc_frames_visit_judge(const uint64_t *v) {
    uint64_t n = (uint64_t)g_gc_chw.frames - v[2];
    if (!v[4]) { g_gc_chw.v_refused++; return; }
    if (v[0] == n && v[1] == g_gc_chw.fsum) { g_gc_chw.v_same++; return; }
    g_gc_chw.v_diff++;
    if (g_gc_chw.said++ < 4)
        fprintf(stderr, "[CHAIN-VISIT] DIFFER rt_gc_frames_visit frames=%lu sum=%lx, the collector's chain frames=%lu sum=%lx\n", (unsigned long)v[0], (unsigned long)v[1], (unsigned long)n,
        (unsigned long)g_gc_chw.fsum);
}
static int gc_chw_line(char *buf, size_t sz) {
    return snprintf(buf, sz,
        "[CHAIN-WALK] SUMMARY runs=%ld whole=%ld frames=%ld offchain-swept=%ld missing=%ld mapdiff=%ld extra-blob-ptrgc=%ld extra-gen-ptrgc=%ld extra-root-ptrgc=%ld extra-plain-ptrgc=%ld beyond-stop"
        "=%ld short=%ld | stop: root=%ld nosite=%ld frameless=%ld unknown-depth=%ld cycle=%ld no-link=%ld no-rise=%ld left-segment=%ld coroutine-start=%ld bad-link=%ld no-entry=%ld top-above=%ld | f"
        "rame-links=%ld links-below=%ld | visitor: s" "ame=%ld differ=%ld refused=%ld | ghost=%ld" "\n", g_gc_chw.runs, g_gc_chw.whole, g_gc_chw.frames, g_gc_chw.offchain, g_gc_chw.missing,
        g_gc_chw.mapdiff, g_gc_chw.xblob, g_gc_chw.xgen, g_gc_chw.xroot, g_gc_chw.xplain, g_gc_chw.beyond, g_gc_chw.shrt, g_gc_chw.s_root, g_gc_chw.s_nosite, g_gc_chw.s_frameless, g_gc_chw.s_unknown,
        g_gc_chw.s_cycle, g_gc_chw.s_nolink, g_gc_chw.s_rise, g_gc_chw.s_left, g_gc_chw.s_costart, g_gc_chw.s_badlink, g_gc_chw.s_noentry, g_gc_chw.s_topabove, g_gc_chw.links, g_gc_chw.links_below,
        g_gc_chw.v_same, g_gc_chw.v_diff, g_gc_chw.v_refused, g_gc_chw.ghost);
}
static long gc_cmdline_len(void) { char ch; long n = 0; int c = open("/proc/self/cmdline", O_RDONLY); if (c < 0) return 0; while (read(c, &ch, 1) == 1) n++; close(c); return n; }
static void gc_chain_walk_report_atexit(void) {
    const char *lg = getenv("SCRIP_GC_CHAIN_LOG");
    int n = gc_chw_line((char *)0, 0);
    char line[n + 1];
    gc_chw_line(line, (size_t)n + 1);
    fputs(line, stderr);
    if (lg && *lg && n > 0) {
        int fd = open(lg, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd >= 0) {
            long k = gc_cmdline_len();
            char cmd[k + 1], out[k + n + 8];
            int c = open("/proc/self/cmdline", O_RDONLY);
            long r = 0;
            if (c >= 0) { r = (long)read(c, cmd, (size_t)k); close(c); }
            if (r < 0) r = 0;
            cmd[r] = 0;
            for (long q = 0; q < r; q++) if (cmd[q] == 0) cmd[q] = ' ';
            int m = snprintf(out, sizeof out, "CMD %s | %s", cmd, line);
            if (m > 0 && m < (int)sizeof out && write(fd, out, (size_t)m) < 0) { }
            close(fd);
        }
    }
}
#if RT_DIAG
static long g_gc_rtccb_heap;
static void gc_walk_print(void) {
    if (!gc_maps_on()) { memset(g_gw, 0, sizeof g_gw); g_gw_lines = 0; g_gw_suppressed = 0; return; }
    if (g_gc_rtccb_heap) fprintf(stderr, "[GC-RTCCB] heap=%ld\n", g_gc_rtccb_heap);
    for (int k = 0; k < GC_REP_POPS; k++) {
        gc_walk_t *g = &g_gw[k];
        if (!g->frames && !g->nomap && !g->s_words && !g->a_words) continue;
        fprintf(stderr,
            "[GC-WALK] pop=%-7s frames=%ld roots=%ld nomap=%ld notab=%ld i_descr=%ld i_heap=%ld i_badtag=%ld i_phase=%ld i_ptr=%ld i_ptr_heap=%ld i_raw=%ld i_raw_heap=%ld i_gap=%ld h_words=%ld h_cel"
            "l_heap=%ld h_raw_heap=%ld s_words=%ld s_cell_heap=%ld s_raw_heap=%ld a_words=%ld a_heap=%ld divergence=%ld ghost_cells=%ld\n", g_gc_rep_popname[k], g->frames, g->roots, g->nomap,
            g->notab, g->i_descr, g->i_heap, g->i_badtag, g->i_phase, g->i_ptr, g->i_ptr_heap, g->i_raw, g->i_raw_heap, g->i_gap, g->h_words, g->h_cell_heap, g->h_raw_heap, g->s_words, g->s_cell_heap,
            g->s_raw_heap, g->a_words, g->a_heap, g->i_raw_heap + g->h_raw_heap + g->s_raw_heap + g->a_heap, g->ghost);
    }
    if (g_gw_suppressed) fprintf(stderr, "[GC-WALK] site lines suppressed=%ld cap=%d\n", g_gw_suppressed, GC_WALK_LINE_CAP);
    memset(g_gw, 0, sizeof g_gw);
    g_gw_lines = 0;
    g_gw_suppressed = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_spine_lost_check(void) {
    long nlost = 0;
    if (!gc_maps_on()) { g_gc_spine_recn = 0; g_gc_spine_seen = 0; return; }
    for (int i = 0; i < g_gc_spine_recn; i++) {
        rt_hblk_t *h = g_gc_spine_rec[i].h;
        if (gc_blk_marked(h)) continue;
        g_gc_spine_lost++;
        nlost++;
        {
            const unsigned char *b = (const unsigned char *)(h + 1);
            long n = (long)h->size - (long)sizeof(rt_hblk_t), m = n < 24 ? (n > 0 ? n : 0) : 24;
            char tx[m + 1];
            int j = 0;
            for (; j < m; j++) tx[j] = (b[j] >= 32 && b[j] < 127) ? (char)b[j] : '.';
            tx[j] = 0;
            fprintf(stderr, "[GC-SPINE-LOST] graph=%s off=%ld at=%p word=%p blk=%p type=%u text=%s\n", g_gc_spine_rec[i].graph ? g_gc_spine_rec[i].graph : "-", g_gc_spine_rec[i].off,
                (const void *)g_gc_spine_rec[i].at, (const void *)*g_gc_spine_rec[i].at, (const void *)h, (unsigned)h->type, tx);
        }
    }
    if (nlost)
        fprintf(stderr,
        "[GC-SPINE-LOST] blocks=%ld raw_spine_words=%ld -- a word on the emitted spine that no map covers points at a block NO ROOT MARKED. ⛔ NECESSARY, NOT SUFFICIENT (cfo 2026-09-20, CFO-114): a"
        " DEAD spill slot holding a stale pointer reads identically, and 12 of the 32 green witnesses in scripts/gc_witnesses print this line while answering their oracle. It names a candidate, neve"
        "r a defect; only the frame map can say whether the slot is live.\n", nlost, g_gc_spine_seen);
    g_gc_spine_recn = 0;
    g_gc_spine_seen = 0;
}
#endif
static const char *gc_chain_stopname(int k) {
    switch (k) {
        case 0:
        return "root";
        case 1:
        return "nosite";
        case 2:
        return "frameless";
        case 3:
        return "unknown-depth";
        case 4:
        return "cycle";
        case 5:
        return "no-link";
        case 6:
        return "no-rise";
        case 7:
        return "left-segment";
        case 8:
        return "coroutine-start";
        case 9:
        return "bad-link";
        case 10:
        return "no-entry";
        case 11:
        return "top-above-segment";
        case 12:
        return "link-below-gap";
        default:
        return "?";
    }
}
typedef struct { gc_chf_t *v; long n, cap; } gc_gap_t;
typedef struct gc_links_args_t {
    const char *f_base;
    const gc_frame_map_t *f_map;
    uint64_t f_link;
    long f_rule;
    const char *s_lo, *s_hi, *prevbase, *glo;
    gc_chf_t *gap_v;
    long gap_n, gap_cap;
    const gc_site_blk_t *sblk;
    long sblk_n;
    uint64_t memo_beta[4];
    const gc_site_blk_t *memo_blk[4];
    long memo_at;
    long *links_ctr;
    long pos_j, pos_k, kind, size;
    const char *cbase, *hdr;
    uint64_t beta;
    const char *v;
    const gc_site_blk_t *b;
    const gc_site_ent_t *ent;
} gc_links_args_t;
_Static_assert(__builtin_offsetof(gc_links_args_t, f_base) == 0 && __builtin_offsetof(gc_links_args_t, f_map) == 8 && __builtin_offsetof(gc_links_args_t, f_link) == 16 &&
    __builtin_offsetof(gc_links_args_t, f_rule) == 24 && __builtin_offsetof(gc_links_args_t, s_lo) == 32 && __builtin_offsetof(gc_links_args_t, s_hi) == 40 &&
    __builtin_offsetof(gc_links_args_t, prevbase) == 48 && __builtin_offsetof(gc_links_args_t, glo) == 56 && __builtin_offsetof(gc_links_args_t, gap_v) == 64 &&
    __builtin_offsetof(gc_links_args_t, gap_n) == 72 && __builtin_offsetof(gc_links_args_t, gap_cap) == 80 && __builtin_offsetof(gc_links_args_t, sblk) == 88 &&
    __builtin_offsetof(gc_links_args_t, sblk_n) == 96 && __builtin_offsetof(gc_links_args_t, memo_beta) == 104 && __builtin_offsetof(gc_links_args_t, memo_blk) == 136 &&
    __builtin_offsetof(gc_links_args_t, memo_at) == 168 && __builtin_offsetof(gc_links_args_t, links_ctr) == 176 && __builtin_offsetof(gc_links_args_t, pos_j) == 184 &&
    __builtin_offsetof(gc_links_args_t, pos_k) == 192 && __builtin_offsetof(gc_links_args_t, kind) == 200 && __builtin_offsetof(gc_links_args_t, size) == 208 &&
    __builtin_offsetof(gc_links_args_t, cbase) == 216 && __builtin_offsetof(gc_links_args_t, hdr) == 224 && __builtin_offsetof(gc_links_args_t, beta) == 232 &&
    __builtin_offsetof(gc_links_args_t, v) == 240 && __builtin_offsetof(gc_links_args_t, b) == 248 && __builtin_offsetof(gc_links_args_t, ent) == 256 && sizeof(gc_chf_t) == 32 &&
    __builtin_offsetof(gc_chf_t, map) == 8 && __builtin_offsetof(gc_chf_t, link) == 16 && __builtin_offsetof(gc_chf_t, rule) == 24 && __builtin_offsetof(gc_chf_t, ext) == 28 &&
    GC_LAY_PTR_FRAME == 4 && GC_LAY_PTR_FRAME_H == 5 && GC_FRAME_MAP_GEN_ANCHOR == 2 && GC_FRAME_MAP_LAYOUT == 8,
    "rt_gc_sw_links_a (rtx_gc.s) reads its argument block, the gap entries and the layout kinds at these offsets");
typedef struct gc_swx_t {
    uint64_t pc;
    const char *r, *rbp, *last;
    gc_chf_t f, cur;
    long phase, i, sub;
    const char *co_body, *glo, *w1lo, *w1hi, *clo, *chi, *w2lo, *w2hi, *pnew;
    long cells;
    const gc_site_ent_t *e;
    uint64_t dpc;
    const char *dr, *drbp;
    long lam, pw;
    void *gw;
    const char **top_graph_p;
} gc_swx_t;
typedef struct {
    const char *lo, *hi, *p, *prevbase, *lastname, *roottop;
    long n;
    int run, visit, stop;
    rt_gc_frame_fn fn;
    void *a;
    gc_chf_t lastf;
    gc_gap_t gap;
    gc_links_args_t la;
    gc_swx_t x;
} gc_sw_t;
_Static_assert(__builtin_offsetof(gc_sw_t, lo) == 0 && __builtin_offsetof(gc_sw_t, hi) == 8 && __builtin_offsetof(gc_sw_t, p) == 16 && __builtin_offsetof(gc_sw_t, prevbase) == 24 &&
    __builtin_offsetof(gc_sw_t, lastname) == 32 && __builtin_offsetof(gc_sw_t, roottop) == 40 && __builtin_offsetof(gc_sw_t, n) == 48 && __builtin_offsetof(gc_sw_t, run) == 56 &&
    __builtin_offsetof(gc_sw_t, stop) == 64 && __builtin_offsetof(gc_sw_t, lastf) == 88 && __builtin_offsetof(gc_sw_t, gap) == 120 && __builtin_offsetof(gc_gap_t, v) == 0 &&
    __builtin_offsetof(gc_gap_t, n) == 8 && __builtin_offsetof(gc_sw_t, la) == 144 && __builtin_offsetof(gc_sw_t, x) == 408 && __builtin_offsetof(gc_swx_t, pc) == 0 &&
    __builtin_offsetof(gc_swx_t, r) == 8 && __builtin_offsetof(gc_swx_t, rbp) == 16 && __builtin_offsetof(gc_swx_t, last) == 24 && __builtin_offsetof(gc_swx_t, f) == 32 &&
    __builtin_offsetof(gc_swx_t, cur) == 64 && __builtin_offsetof(gc_swx_t, phase) == 96 && __builtin_offsetof(gc_swx_t, i) == 104 && __builtin_offsetof(gc_swx_t, sub) == 112 &&
    __builtin_offsetof(gc_swx_t, co_body) == 120 && __builtin_offsetof(gc_swx_t, glo) == 128 && __builtin_offsetof(gc_swx_t, w1lo) == 136 && __builtin_offsetof(gc_swx_t, pnew) == 184 &&
    __builtin_offsetof(gc_swx_t, cells) == 192 && __builtin_offsetof(gc_swx_t, e) == 200 && __builtin_offsetof(gc_swx_t, dpc) == 208 && __builtin_offsetof(gc_swx_t, lam) == 232 &&
    __builtin_offsetof(gc_swx_t, pw) == 240 && __builtin_offsetof(gc_swx_t, gw) == 248 && __builtin_offsetof(gc_swx_t, top_graph_p) == 256 && __builtin_offsetof(gc_chf_t, rule) == 24 &&
    __builtin_offsetof(gc_chf_t, ext) == 28 && __builtin_offsetof(gc_site_ent_t, kind) == 8 && __builtin_offsetof(gc_site_ent_t, rule) == 10 && __builtin_offsetof(gc_site_ent_t, depth) == 12 &&
    __builtin_offsetof(gc_site_blk_t, map) == 8 && __builtin_offsetof(gc_site_blk_t, link) == 56 && __builtin_offsetof(gc_frame_map_t, frame_bytes) == 4 &&
    __builtin_offsetof(gc_frame_map_t, header_bytes) == 8 && __builtin_offsetof(gc_frame_map_t, flags) == 12 && __builtin_offsetof(gc_frame_map_t, graph_name) == 16 &&
    __builtin_offsetof(gc_frame_map_t, value_bytes) == 24 && GC_FRAME_MAP_ROOT == 1u && GC_FRAME_MAP_BLOB == 4u && GC_FRAME_MAP_LAYOUT == 8u,
    "rt_gc_sw_step_a (rtx_gc.s) reads and writes the walk state, the link-scan arguments, the sites, the blocks and the frame maps at these offsets");
static void gc_gap_open(gc_gap_t *g) {
    g->cap = 24;
    g->v = (gc_chf_t *)gcbk_alloc((size_t)g->cap * sizeof *g->v);
    g->n = 0;
    if (!g->v) { fprintf(stderr, "[ZGC] the retained-frame set of a chain gap could not be opened\n"); abort(); }
}
static void gc_gap_close(gc_gap_t *g) { if (g->v) gcbk_drop((void *)g->v); g->v = (gc_chf_t *)0; g->n = 0; g->cap = 0; }
static void gc_gap_grow(gc_gap_t *g) {
    gc_chf_t *nv = (gc_chf_t *)gcbk_alloc((size_t)(2 * g->cap) * sizeof *nv);
    if (!nv) { fprintf(stderr, "[ZGC] the retained-frame set of one chain gap could not grow past %ld entries\n", g->cap); abort(); }
    memcpy(nv, g->v, (size_t)g->n * sizeof *nv);
    gcbk_drop((void *)g->v);
    g->v = nv;
    g->cap *= 2;
}
static void gc_gap_push(gc_gap_t *g, const char *base, const gc_frame_map_t *m, uint64_t link) {
    if (g->n == g->cap) gc_gap_grow(g);
    g->v[g->n].base = base;
    g->v[g->n].map = m;
    g->v[g->n].link = link;
    g->v[g->n].rule = -1;
    g->v[g->n].ext = 0;
    g->n++;
}
static void gc_chain_tally(const gc_sw_t *s) {
    if (!g_gc_mk.chain) return;
    if (!g_gc_chw.runs) atexit(gc_chain_walk_report_atexit);
    if (!g_gc_chain_reg) { g_gc_chain_reg = 1; atexit(gc_chain_report_atexit); }
    g_gc_chw.runs++;
    g_gc_chw.frames += s->n;
    switch (s->stop) {
        case 0:
        g_gc_chw.s_root++;
        break;
        case 1:
        g_gc_chw.s_nosite++;
        g_gc_chain_nosite++;
        break;
        case 2:
        g_gc_chw.s_frameless++;
        g_gc_chain_frameless++;
        break;
        case 3:
        g_gc_chw.s_unknown++;
        break;
        case 4:
        g_gc_chw.s_cycle++;
        break;
        case 5:
        g_gc_chw.s_nolink++;
        break;
        case 6:
        g_gc_chw.s_rise++;
        break;
        case 8:
        g_gc_chw.s_costart++;
        break;
        case 9:
        g_gc_chw.s_badlink++;
        break;
        case 10:
        g_gc_chw.s_noentry++;
        break;
        case 11:
        g_gc_chw.s_topabove++;
        break;
        case 12:
        g_gc_chw.s_badlink++;
        break;
        default:
        g_gc_chw.s_left++;
        break;
    }
    if (s->stop == 0 || (!s->run && (s->stop == 7 || s->stop == 8))) { g_gc_chw.whole++; g_gc_chain_ok++; } else g_gc_chw.shrt++;
}
static void gc_chain_refuse(const gc_sw_t *s, const gc_ent_t *ent, const char *lo, const char *hi) {
    uint64_t lpc = 0;
    const char *lr = (const char *)0, *lrbp = (const char *)0;
    Dl_info di;
    fprintf(stderr,
        "[GC-CHAIN] REFUSED: the chain over the maps stops short (%s) after %ld frame(s); entry pc=%p R=%p, segment [%p,%p). The collector walks only the chain (ARCH-GC 13.9 ONE): it never skips.\n",
        gc_chain_stopname(s->stop), s->n, ent ? ent->pc : (const void *)0, ent ? (const void *)ent->r : (const void *)0, (const void *)lo, (const void *)hi);
    if (s->n > 0) {
        const gc_site_ent_t *le = (const gc_site_ent_t *)0;
        const gc_site_blk_t *lb;
        gc_chain_link(s->lastf.base, s->lastf.link, &lpc, &lr, &lrbp);
        lb = gc_site_find(lpc, &le);
        fprintf(stderr, "[GC-CHAIN]   last frame base=%p map=%s; its link: pc=%p image+%lx (%s%+ld%s) R=%p rbp=%p\n", (const void *)s->lastf.base,
            s->lastf.map->graph_name ? s->lastf.map->graph_name : "?", (const void *)(uintptr_t)lpc,
            dladdr((const void *)(uintptr_t)lpc, &di) ? (unsigned long)(lpc - (uint64_t)(uintptr_t)di.dli_fbase) : 0UL,
            dladdr((const void *)(uintptr_t)lpc, &di) && di.dli_sname ? di.dli_sname : (lb ? "SITE" : "?"),
            dladdr((const void *)(uintptr_t)lpc, &di) && di.dli_saddr ? (long)(lpc - (uint64_t)(uintptr_t)di.dli_saddr) : 0L, lb && lb->map && lb->map->graph_name ? lb->map->graph_name : "",
            (const void *)lr, (const void *)lrbp);
    }
    if (s->n > 0)
        fprintf(stderr, "[GC-CHAIN]   last frame %ld base=%p map=%s flags=%u rule=%d fb=%u hb=%u\n", s->n - 1, (const void *)s->lastf.base, s->lastf.map->graph_name ? s->lastf.map->graph_name : "?",
        s->lastf.map->flags, s->lastf.rule, s->lastf.map->frame_bytes, s->lastf.map->header_bytes);
    rt_bomb("gc: the chain over the maps stopped short -- see the [GC-CHAIN] lines; ARCH-GC-COMPILE-TIME-FRAME-MAPS.md 13.9 ONE");
}
static void gc_sw_frame(gc_sw_t *s, const gc_chf_t *f) {
    const gc_frame_map_t *m = f->map;
    const char *base = f->base, *lo = s->lo, *hi = s->hi;
#if RT_DIAG
    gc_walk_t *g = &g_gw[g_gc_rep_pop];
    int k = g_gc_rep_pop;
#endif
    s->lastf = *f;
    if (s->fn) s->fn((const void *)m, base, s->a);
    if (s->visit) {
        if (gc_chain_check_on()) g_gc_chw.fsum = g_gc_chw.fsum * 31u + ((uint64_t)(uintptr_t)base ^ (uint64_t)(uintptr_t)m);
#if RT_DIAG
        g->frames++;
        if (s->n == 0 && !g_gc_top_graph) g_gc_top_graph = m->graph_name ? m->graph_name : "?";
#endif
        if (m->flags & GC_FRAME_MAP_BLOB) {
            const char *c = base - (long)m->frame_bytes, *top = base + (long)m->header_bytes;
            if (top > hi) top = hi;
#if RT_DIAG
            if (g_gc_mk.maps && base + 24 <= hi) {
                const char *const *h = (const char *const *)base;
                int up = h[0] > base && h[0] < hi, r1 = gc_code_range(h[1]), r2 = gc_code_range(h[2]);
                if (!(up && r1 && r2)) g->ghost++;
                if (gc_maps_verbose())
                    fprintf(gc_maps_log(), "[GC-WALK-CELL] pop=%s cell=%p graph=%s frame_bytes=%u header_bytes=%u value_bytes=%lu flags=%u hdr=%p,%p,%p rbp_up=%d code=%d,%d\n", g_gc_rep_popname[k],
                    (const void *)c, m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, (unsigned long)m->value_bytes, m->flags, (const void *)h[0], (const void *)h[1],
                    (const void *)h[2], up, r1, r2);
            }
#endif
            gc_walk_words_x(s->p, c, 0, lo, m->graph_name, c);
            gc_visit_frame_cells_x(base, m, lo, hi);
            s->p = top;
        } else {
            const char *slo = (base < s->p) ? s->p : base, *hlo = base + (long)m->value_bytes + 16, *hhi = hlo + (long)m->header_bytes;
            if (hhi > hi) hhi = hi;
#if RT_DIAG
            if (g_gc_mk.maps && gc_maps_verbose())
                fprintf(gc_maps_log(), "[GC-WALK-CELL] pop=%s cell=%p graph=%s frame_bytes=%u header_bytes=%u value_bytes=%lu flags=%u\n", g_gc_rep_popname[k],
                (const void *)(base + (long)m->value_bytes), m->graph_name ? m->graph_name : "?", m->frame_bytes, m->header_bytes, (unsigned long)m->value_bytes, m->flags);
            {
                long before = g->s_raw_heap;
                gc_walk_words_x(s->p, slo, 0, lo, m->graph_name, base);
                if (s->n == 0 && g->s_raw_heap > before && gc_walk_census_on()) gc_walk_dump(s->p, slo, m->graph_name);
            }
#else
            gc_walk_words_x(s->p, slo, 0, lo, m->graph_name, base);
#endif
            if (m->flags & GC_FRAME_MAP_LAYOUT) gc_visit_frame_cells_x(base, m, lo, (f->ext > 0 && base + 16L * f->ext < hi) ? base + 16L * f->ext : hi);
#if RT_DIAG
            else g->notab++;
#endif
            gc_walk_words_x(hlo, hhi, 1, lo, m->graph_name, base);
            s->p = hhi;
        }
#if RT_DIAG
        if (m->flags & GC_FRAME_MAP_ROOT) g->roots++;
#endif
    }
    s->n++;
    s->lastname = m->graph_name;
}
static int gc_sw_links_c(gc_sw_t *s, gc_gap_t *g, const gc_chf_t *f, const char *glo) {
    const gc_frame_map_t *m = f->map;
    const uint64_t *t;
    if (!m || !(m->flags & GC_FRAME_MAP_LAYOUT)) return 1;
    t = (const uint64_t *)(m + 1);
    for (long j = 0; j < (long)t[0]; j++) {
        uint64_t q = t[1 + j];
        unsigned lk = GC_LAY_KIND(q);
        if (lk != GC_LAY_PTR_FRAME && lk != GC_LAY_PTR_FRAME_H) continue;
        for (int k = 0; k < GC_LAY_SIZE(q); k += 8) {
            const char *c = f->base + GC_LAY_OFF(q) + k, *v = *(const char *const *)c, *why = (const char *)0;
            const char *hdr = (const char *)0;
            uint64_t beta = 0;
            const gc_site_blk_t *b;
            if (!v) continue;
            if (f->rule == -1 && !f->link && (v < s->lo || v >= s->hi)) continue;
            if (lk == GC_LAY_PTR_FRAME_H) {
                hdr = v;
                if (hdr < s->lo || hdr + 48 > s->hi || hdr >= f->base) why = "the generator header lies outside the segment or not below the frame that links it";
                else beta = *(const uint64_t *)(hdr + 32);
            } else beta = *(const uint64_t *)(c + 8);
            if (why) b = (const gc_site_blk_t *)0;
            else b = gc_code_block(beta);
            if (why) ;
            else if (!b) why = "its beta word names no registered graph";
            else if (hdr && !(b->map->flags & GC_FRAME_MAP_GEN_ANCHOR)) why = "the generator's resume cell names no generator graph";
            else if (hdr && (v = hdr - (long)b->map->frame_bytes) < s->lo) why = "the target lies outside the segment";
            else if (v < s->lo || v + b->map->value_bytes + 16 + b->map->header_bytes > s->hi) why = "the target lies outside the segment";
            else if (v >= f->base) why = "the target is not below the frame that links it";
            else if (!GC_LINK_SET(b->link)) why = "the target's map states no link word";
            else {
                const gc_site_ent_t *e = (const gc_site_ent_t *)0;
                const gc_site_blk_t *xb = gc_site_find(*(const uint64_t *)(v + GC_LINK_PC(b->link)), &e);
                if (!xb || !e || xb->map != m) why = "the target's return word is not a site of the frame that links it";
            }
            if (why) {
                if (gc_chain_check_on() && g_gc_chw.said_link++ < 4) {
                    const gc_site_ent_t *xe = (const gc_site_ent_t *)0;
                    const gc_site_blk_t *xb = (b && v >= s->lo && v + 16 <= s->hi && GC_LINK_SET(b->link)) ? gc_site_find(*(const uint64_t *)(v + GC_LINK_PC(b->link)), &xe) : (const gc_site_blk_t *)0;
                    fprintf(stderr, "[CHAIN-WALK] FRAME-LINK REFUSED: %s (linking frame %s base=%p slot +%ld holds %p, beta %p names %s, its return word lands in %s)\n", why,
                        m->graph_name ? m->graph_name : "?", (const void *)f->base, (long)(c - f->base), (const void *)v, (const void *)(uintptr_t)beta,
                        (b && b->map && b->map->graph_name) ? b->map->graph_name : "?", (xb && xb->map && xb->map->graph_name) ? xb->map->graph_name : "no site");
                }
                s->stop = 9;
                return 0;
            }
            if (v == s->prevbase) continue;
            if (v < glo) {
                if (gc_chain_check_on() && g_gc_chw.said_link++ < 4)
                    fprintf(stderr,
                    "[CHAIN-WALK] FRAME-LINK REFUSED: the target lies below the gap of the frame that links it and is not the chain frame just below (linking frame %s base=%p slot +%ld holds %p, bet"
                    "a " "%p names %s fb=%u, the chain frame just below is %p)\n", m->graph_name ? m->graph_name : "?", (const void *)f->base, (long)(c - f->base), (const void *)v,
                    (const void *)(uintptr_t)beta, b->map->graph_name ? b->map->graph_name : "?", b->map->frame_bytes, (const void *)s->prevbase);
                g_gc_chw.links_below++;
                s->stop = 12;
                return 0;
            }
            gc_gap_push(g, v, b->map, b->link);
            g_gc_chw.links++;
        }
    }
    return 1;
}
static int gc_sw_links(gc_sw_t *s, gc_gap_t *g, const gc_chf_t *f, const char *glo) {
    extern long rt_gc_sw_links_a(gc_links_args_t *);
    gc_links_args_t *a = &s->la;
    a->f_base = f->base;
    a->f_map = f->map;
    a->f_link = f->link;
    a->f_rule = f->rule;
    a->s_lo = s->lo;
    a->s_hi = s->hi;
    a->prevbase = s->prevbase;
    a->glo = glo;
    a->sblk = g_gc_sblk;
    a->sblk_n = g_gc_sblk_n;
    a->links_ctr = &g_gc_chw.links;
    a->pos_j = 0;
    a->pos_k = 0;
    for (;;) {
        long r;
        a->gap_v = g->v;
        a->gap_n = g->n;
        a->gap_cap = g->cap;
        r = rt_gc_sw_links_a(a);
        g->n = a->gap_n;
        if (r == 1) return 1;
        if (r == 4) { gc_gap_grow(g); continue; }
        return gc_sw_links_c(s, g, f, glo);
    }
}
static int gc_sw_chain_frame(gc_sw_t *s, const gc_chf_t *f) {
    gc_gap_t *g = &s->gap;
    const char *glo = s->p;
    long i;
    g->n = 0;
    if (!gc_sw_links(s, g, f, glo)) return 0;
    for (i = 0; i < g->n; i++) { gc_chf_t e = g->v[i]; if (!gc_sw_links(s, g, &e, glo)) return 0; }
    if (g->n > 1) { long u = 1; qsort(g->v, (size_t)g->n, sizeof *g->v, gc_chf_cmp); for (i = 1; i < g->n; i++) if (g->v[i].base != g->v[u - 1].base) g->v[u++] = g->v[i]; g->n = u; }
    for (i = 0; i < g->n; i++) gc_sw_frame(s, &g->v[i]);
    gc_sw_frame(s, f);
    s->prevbase = f->base;
    return 1;
}
static int gc_sw_c_step(gc_sw_t *s, const scrip_coctx_t *sc) {
    gc_swx_t *x = &s->x;
    gc_chx_t w;
    const char *hi = s->hi;
    gc_chf_t f;
    memset(&w, 0, sizeof w);
    if (x->r > hi || (x->last && x->r < x->last)) { s->stop = 7; return 0; }
    gc_chain_find(&w, x->pc, x->r, x->rbp, hi);
    if (w.st == 6) {
        const gc_site_blk_t *bb = gc_body_block(sc);
        const char *gb = bb ? sc->gen_h - (long)bb->map->frame_bytes : (const char *)0;
        if (!bb || gb < w.r || gb > hi) { s->stop = 1; return 0; }
        if (x->last && gb <= x->last) { s->stop = 6; return 0; }
        f.base = gb;
        f.map = bb->map;
        f.link = bb->link;
        f.rule = -1;
        f.ext = 0;
        if (!gc_sw_chain_frame(s, &f)) return 0;
        x->last = gb;
        if (!gc_chain_link(gb, bb->link, &x->pc, &x->r, &x->rbp)) { s->stop = 5; return 0; }
        return 1;
    }
    if (w.st != 1) { s->stop = w.st == 3 ? 2 : w.st == 4 ? 3 : w.st == 5 ? 4 : w.st == 7 ? 8 : 1; return 0; }
    if (x->last && w.base <= x->last) { s->stop = 6; return 0; }
    f.base = w.base;
    f.map = w.b->map;
    f.link = w.b->link;
    f.rule = GC_SITE_RULE(w.e);
    f.ext = GC_SITE_EXT(w.e);
    if (!gc_sw_chain_frame(s, &f)) return 0;
    x->last = w.base;
    if (w.b->map->flags & GC_FRAME_MAP_ROOT) {
        const char *rt = w.base + (long)w.b->map->value_bytes + 16 + (long)w.b->map->header_bytes;
        if (s->run && rt > hi) { s->stop = 11; return 0; }
        s->roottop = rt;
    }
    if (x->co_body && w.base == x->co_body) { s->stop = 8; return 0; }
    if (!gc_chain_link(w.base, w.b->link, &x->pc, &x->r, &x->rbp)) { s->stop = (w.b->map->flags & GC_FRAME_MAP_ROOT) ? 0 : 5; return 0; }
    if ((w.b->map->flags & GC_FRAME_MAP_ROOT) && !gc_site_find(x->pc, (const gc_site_ent_t **)0)) { s->stop = 0; return 0; }
    return 1;
}
static void gc_sw_run(gc_sw_t *s, const gc_ent_t *ent, const scrip_coctx_t *sc) {
    extern long rt_gc_sw_step_a(gc_sw_t *);
    gc_swx_t *x = &s->x;
    int fast = s->visit && !s->fn && !g_gc_mk.maps && !g_gc_mk.census && !g_gc_mk.chain;
    x->pc = (uint64_t)(uintptr_t)ent->pc;
    x->r = ent->r;
    x->rbp = (const char *)ent->rbp;
    x->last = (const char *)0;
    x->phase = 0;
    x->sub = 0;
    x->co_body = sc ? scrip_co_body_base(sc) : (const char *)0;
    for (;;) {
        long rc = fast ? rt_gc_sw_step_a(s) : -2;
        if (rc == 0) continue;
        if (rc == -1) return;
        if (rc == -2) { x->phase = 0; x->sub = 0; if (!gc_sw_c_step(s, sc)) return; continue; }
        if (rc == -3) { gc_gap_grow(&s->gap); continue; }
        gc_run_verdicts((DESCR_t *)rc);
    }
}
static void gc_sw_go(gc_sw_t *s, const char *lo0, const char *hi0, const gc_ent_t *ent, const scrip_coctx_t *sc, int run, int visit, rt_gc_frame_fn fn, void *a) {
    const char *img = (sc && sc->image && sc->image_map) ? sc->image + sc->image_off : (const char *)0;
    memset(s, 0, sizeof *s);
    s->lo = (const char *)(((uintptr_t)lo0 + 7u) & ~(uintptr_t)7u);
    s->hi = hi0;
    s->p = s->lo;
    s->run = run;
    s->visit = visit;
    s->fn = fn;
    s->a = a;
    s->lastname = "-";
    s->la.s_lo = s->lo;
    s->la.s_hi = s->hi;
    s->la.sblk = g_gc_sblk;
    s->la.sblk_n = g_gc_sblk_n;
    s->la.links_ctr = &g_gc_chw.links;
#if RT_DIAG
    s->x.gw = (void *)&g_gw[g_gc_rep_pop];
    s->x.top_graph_p = &g_gc_top_graph;
#endif
    gc_gap_open(&s->gap);
    if (ent) gc_sw_run(s, ent, sc);
    if (img && img >= s->lo && img < s->hi && (!s->n || s->prevbase < img) && s->stop != 9) { gc_chf_t f = { img, (const gc_frame_map_t *)sc->image_map, 0, -1, 0 }; gc_sw_chain_frame(s, &f); }
    if (visit) {
        int root = s->roottop != (const char *)0 || (s->n > 0 && (s->lastf.map->flags & GC_FRAME_MAP_ROOT));
        if (!root && s->p + 8 <= s->hi) gc_walk_words_x(s->p, s->hi, 0, s->lo, s->lastname, s->p);
#if RT_DIAG
        if (s->n == 0) g_gw[g_gc_rep_pop].nomap++;
#endif
    }
    gc_gap_close(&s->gap);
}
static long gc_sw_segment_fn(const char *lo0, const char *hi0, const gc_ent_t *ent, const scrip_coctx_t *sc, int run, rt_gc_frame_fn fn, void *a) {
    gc_sw_t s;
    gc_sw_go(&s, lo0, hi0, ent, sc, run, 0, fn, a);
    if (ent && s.stop != 0 && (run || (s.stop != 7 && s.stop != 8))) return -1;
    return s.n;
}
static void gc_program_top_refuse(const gc_ent_t *ent, const char *root_top, const char *lo, const char *hi) {
    fprintf(stderr, "[GC-CHAIN] REFUSED: the walk from entry pc=%p R=%p over [%p,%p) found the root frame ending at %p; the adopted program top is %p (%+ld bytes)\n", ent ? ent->pc : (const void *)0,
        ent ? (const void *)ent->r : (const void *)0, (const void *)lo, (const void *)hi, (const void *)root_top, (const void *)g_gc_program_top,
        g_gc_program_top ? (long)(root_top - g_gc_program_top) : 0L);
    rt_bomb("gc: a walk found the root frame away from the adopted program top -- an adoption road named the wrong top; the collector learns nothing from a walk");
}
static long gc_visit_segment(const char *lo0, const char *hi0, const gc_ent_t *ent, const scrip_coctx_t *sc, int run) {
    gc_sw_t s;
#if RT_DIAG
    if (g_gc_mk.maps) gc_maps_report_range(lo0, hi0);
#endif
    gc_sw_go(&s, lo0, hi0, ent, sc, run, 1, (rt_gc_frame_fn)0, (void *)0);
    if (!ent && run && g_gc_program_top && lo0 < g_gc_program_top) s.stop = 10;
    gc_chain_tally(&s);
    if ((ent || s.stop == 10) && s.stop != 0 && (run || (s.stop != 7 && s.stop != 8))) gc_chain_refuse(&s, ent, lo0, hi0);
    if (run && s.roottop && (const char *)(uintptr_t)s.roottop != g_gc_program_top) gc_program_top_refuse(ent, (const char *)(uintptr_t)s.roottop, lo0, hi0);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static long g_gc_cas_bytes = 0;
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_root_cas(void) {
    extern long rt_cas_gc_roots(void);
#if RT_DIAG
    g_gc_cas_bytes = rt_cas_gc_roots();
#else
    (void)rt_cas_gc_roots();
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_root_gva(void) { g_gc_mk.rng_p = (DESCR_t *)RT_GVA_VA; g_gc_mk.rng_n = (long)g_sxt_gva_n; g_gc_mk.draining = 1; gc_wl_drain(); g_gc_mk.draining = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
_Static_assert(sizeof(long) == 8,
    "THE FORCED-RELOCATION PLANT ADDS A GAP ONLY WHILE EVERY REMAINING LIVE BYTE STILL FITS THE COMMITTED WINDOW (cto 2026-09-23, CTO-156): the gap was checked against the ONE block it displaced, so"
    " on a mostly-live heap the gaps accumulated and the last blocks were forwarded past g_hp_end into uncommitted reserve -- the up-mover memmove faulted inside the collection (benchmark_nrev, benc"
    "hmark_nreverse, last_call_nreverse_large_1 and benchmark_meta_qsort read CRASH under SCRIP_GC_RELOC=1 in mode 4 on every tree, red under the plant and green without it and under the flip plant)"
    "; rel_rem is the live bytes not yet forwarded, this block included");
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifdef SCRIP_GC_AUDIT_B
#if RT_DIAG
static const rt_hblk_t *gc_audit_b_blk_of(const char *p) { return (const rt_hblk_t *)gc_blk_of(p); }
static const rt_hblk_t *gc_audit_b_blk_at(long i) { return (i >= 0 && i < g_gc_nblk && g_gc_idx) ? (const rt_hblk_t *)g_gc_idx[i] : (const rt_hblk_t *)0; }
static long gc_audit_b_birth_of(const char *blk, char *buf, long cap) {
    long bk = gc_birth_find((char *)blk);
    if (bk < 0) return 0;
    {
        Dl_info d0, d1;
        gc_vac_t *v = &g_gc_vac[bk];
        const char *s0 = (dladdr(v->ra_site, &d0) && d0.dli_sname) ? d0.dli_sname : "?";
        const char *s1 = (v->ra_from && dladdr(v->ra_from, &d1) && d1.dli_sname) ? d1.dli_sname : "?";
        return (long)snprintf(buf, (size_t)cap, "birth=#%ld by=%s from=%s born_at=arena+%ld", v->serial, s0, s1, (long)(v->at - g_hp_arena));
    }
}
static int gc_audit_b_xr_cmp(const void *a, const void *b) { const char *x = ((const gc_audit_b_xr_t *)a)->lo, *y = ((const gc_audit_b_xr_t *)b)->lo; return x < y ? -1 : (x > y ? 1 : 0); }
static long gc_audit_b_lay_ranges(const char *anchor, const gc_frame_map_t *m, const char *lo, const char *hi, gc_audit_b_xr_t *xr, long n, long cap) {
    const uint64_t *t = (const uint64_t *)(m + 1);
    long ne = (long)t[0];
    for (long j = 0; j < ne && n < cap; j++) {
        uint64_t q = t[1 + j];
        unsigned kind = GC_LAY_KIND(q);
        const char *w = anchor + GC_LAY_OFF(q), *e = w + GC_LAY_SIZE(q);
        if (kind != GC_LAY_RAW && kind != GC_LAY_PTR_CODE && kind != GC_LAY_PTR_FRAME && kind != GC_LAY_PTR_FRAME_H) continue;
        if (w < lo) w = lo;
        if (e > hi) e = hi;
        if (w < e) { xr[n].lo = w; xr[n].hi = e; n++; }
    }
    return n;
}
typedef struct { gc_audit_b_xr_t *xr; long n, cap; const char *lo, *hi; } gc_audit_b_rg_t;
static void gc_audit_b_range_cb(const void *map, const char *base, void *a) {
    gc_audit_b_rg_t *r = (gc_audit_b_rg_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    if (r->n >= r->cap) return;
    if (m->flags & GC_FRAME_MAP_BLOB) { r->n = gc_audit_b_lay_ranges(base, m, r->lo, r->hi, r->xr, r->n, r->cap); return; }
    {
        const char *hlo = base + (long)m->value_bytes + 16, *hhi = hlo + (long)m->header_bytes;
        if (hhi > r->hi) hhi = r->hi;
        if (m->flags & GC_FRAME_MAP_LAYOUT) r->n = gc_audit_b_lay_ranges(base, m, r->lo, r->hi, r->xr, r->n, r->cap);
        if (hlo < hhi && r->n < r->cap) { r->xr[r->n].lo = hlo; r->xr[r->n].hi = hhi; r->n++; }
    }
}
static long gc_audit_b_frame_ranges(const char *lo0, const char *hi, const gc_ent_t *ent, const scrip_coctx_t *sc, gc_audit_b_xr_t *xr, long n, long cap) {
    gc_audit_b_rg_t r = { xr, n, cap, (const char *)(((uintptr_t)lo0 + 7u) & ~(uintptr_t)7u), hi };
    if (ent) gc_sw_segment_fn(lo0, hi, ent, sc, 0, gc_audit_b_range_cb, (void *)&r);
    return r.n;
}
static long gc_audit_b_shim(char *floor) {
    gc_audit_b_skip_t sk[32];
    gc_audit_b_t v;
    gc_seg_it_t it;
    char *lo, *hi;
    long n = 0, k = 0, i, nstat, nx = 0, xcap = 0, nrg = 2;
    int pop, saved = g_gc_seg_main;
    gc_audit_b_xr_t *xr = (gc_audit_b_xr_t *)0;
    uint64_t upc = 0;
    const char *ur = (const char *)0, *urbp = (const char *)0;
    gc_ent_t ue;
    int uhas = gc_chain_entry_from_c(&upc, &ur, &urbp);
    if (!g_hp_arena || !g_gc_idx) return -1;
    ue.pc = (const void *)(uintptr_t)upc;
    ue.r = ur;
    ue.rbp = (const void *)urbp;
    gc_static_segs_init();
    nrg += g_gc_nseg;
    for (scrip_coctx_t *c = scrip_co_gc_head(); c; c = c->gc_next) nrg++;
    gc_audit_b_rgn_t rg[nrg];
    gc_ent_t re[nrg];
    const scrip_coctx_t *rs[nrg];
    int rh[nrg];
    for (i = 0; i < g_gc_nseg; i++) { rg[n].lo = g_gc_segs[i].lo; rg[n].hi = g_gc_segs[i].hi; rg[n].pop = "static"; rg[n].name = "writable-PT_LOAD"; rg[n].xr = xr; rg[n].nxr = 0; n++; }
    nstat = n;
    gc_seg_begin(&it, floor);
    while (n < nrg && gc_seg_next(&it, &lo, &hi, &pop)) {
        rg[n].lo = lo;
        rg[n].hi = hi;
        rg[n].pop = "stack";
        rg[n].name = g_gc_rep_popname[pop];
        rg[n].xr = xr;
        rg[n].nxr = 0;
        re[n] = it.prbp ? (gc_ent_t){ (const void *)0, lo, it.prbp } : ue;
        rs[n] = it.sc;
        rh[n] = it.prbp ? 1 : uhas;
        xcap += (long)(hi - lo) / 8 + 1;
        n++;
    }
    g_gc_seg_main = saved;
    if (xcap > 0) {
        xr = (gc_audit_b_xr_t *)gcbk_alloc((size_t)xcap * sizeof(*xr));
        for (i = nstat; i < n; i++) {
            long s0 = nx;
            nx = gc_audit_b_frame_ranges(rg[i].lo, rg[i].hi, rh[i] ? &re[i] : (const gc_ent_t *)0, rs[i], xr, nx, xcap);
            qsort((void *)(xr + s0), (size_t)(nx - s0), sizeof(*xr), gc_audit_b_xr_cmp);
            rg[i].xr = xr + s0;
            rg[i].nxr = nx - s0;
        }
    }
    sk[k].at = (const void *)&g_hp_fr;
    sk[k].bytes = (long)sizeof g_hp_fr;
    sk[k].name = "g_hp_fr";
    k++;
    sk[k].at = (const void *)&g_hp_arena;
    sk[k].bytes = (long)sizeof g_hp_arena;
    sk[k].name = "g_hp_arena";
    k++;
    sk[k].at = (const void *)&g_hp_gcline;
    sk[k].bytes = (long)sizeof g_hp_gcline;
    sk[k].name = "g_hp_gcline";
    k++;
    sk[k].at = (const void *)&g_hp_win;
    sk[k].bytes = (long)sizeof g_hp_win;
    sk[k].name = "g_hp_win";
    k++;
    sk[k].at = (const void *)&g_hp_wend;
    sk[k].bytes = (long)sizeof g_hp_wend;
    sk[k].name = "g_hp_wend";
    k++;
    sk[k].at = (const void *)&g_hp_cap_end;
    sk[k].bytes = (long)sizeof g_hp_cap_end;
    sk[k].name = "g_hp_cap_end";
    k++;
    sk[k].at = (const void *)&g_hp_qlo;
    sk[k].bytes = (long)sizeof g_hp_qlo;
    sk[k].name = "g_hp_qlo";
    k++;
    sk[k].at = (const void *)&g_hp_qhi;
    sk[k].bytes = (long)sizeof g_hp_qhi;
    sk[k].name = "g_hp_qhi";
    k++;
    sk[k].at = (const void *)&g_gc_mk;
    sk[k].bytes = (long)sizeof g_gc_mk;
    sk[k].name = "g_gc_mk";
    k++;
    sk[k].at = (const void *)g_gc_spine_rec;
    sk[k].bytes = (long)sizeof g_gc_spine_rec;
    sk[k].name = "g_gc_spine_rec";
    k++;
    v.blk_of = gc_audit_b_blk_of;
    v.blk_at = gc_audit_b_blk_at;
    v.birth_of = gc_birth_on() ? gc_audit_b_birth_of : (long (*)(const char *, char *, long))0;
    v.alo = g_hp_arena;
    v.ahi = g_hp_top;
    v.run = g_gc_runs + 1;
    v.nblk = g_gc_nblk;
    v.rgn = rg;
    v.nrgn = n;
    v.skip = sk;
    v.nskip = k;
    { extern const char *core_gc_audit_nonref(const char *p); v.owner_nonref = core_gc_audit_nonref; }
    v.hlo = g_hp_arena;
    v.hhi = g_hp_cap_end;
    { long r = gc_audit_b_collect(&v); if (xr) gcbk_drop((void *)xr); return r; }
}
#endif
#endif
#if RT_DIAG
static void gc_assert_report_done(void) {
    const char *e = getenv("SCRIP_GC_ASSERT_FATAL");
    if (e && *e == '1') { fprintf(stderr, "[ZGC-ASSERT] SCRIP_GC_ASSERT_FATAL=1: aborting on the first violated assertion.\n"); abort(); }
}
static void gc_assert_check(void) {
    long nd = 0, ni = 0;
    for (long i = 0; i < g_gc_nblk; i++) { uint16_t f = g_gc_idx[i]->flags; if (!gc_blk_marked(g_gc_idx[i])) continue; nd += (f & HBF_ASSERT_DEAD) ? 1 : 0; ni += (f & HBF_ASSERT_INST) ? 1 : 0; }
    if (!nd && !ni) return;
    for (long i = 0; i < g_gc_nblk; i++) {
        rt_hblk_t *h = g_gc_idx[i];
        if (!gc_blk_marked(h)) continue;
        if (h->flags & HBF_ASSERT_DEAD) {
            fprintf(stderr, "[ZGC-ASSERT] DEAD-ASSERTED block kind=%u/%s size=%u at arena+%ld is REACHABLE at collection #%ld: a root or a live block still holds what the program asserted dead\n",
                (unsigned)h->type, HB_KIND_NAME(h->type), h->size, (long)((char *)h - g_hp_arena), g_gc_runs + 1);
            gc_assert_report_done();
        }
        if (h->flags & HBF_ASSERT_INST) {
            const DESCR_t *v = (const DESCR_t *)(h + 1);
            long want = (long)v[1].i, found = 0;
            uint16_t t = (uint16_t)v[0].i;
            for (long j = 0; j < g_gc_nblk; j++) if (gc_blk_marked(g_gc_idx[j]) && g_gc_idx[j]->type == t) found++;
            if (found != want) {
                fprintf(stderr, "[ZGC-ASSERT] INSTANCES kind=%u/%s expected %ld live, the trace found %ld at collection #%ld\n", (unsigned)t, HB_KIND_NAME(t), want, found, g_gc_runs + 1);
                gc_assert_report_done();
            }
        }
    }
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_trail_refuse(const char *hb) {
    fprintf(stderr, "[GC-TRAIL] REFUSED: a heap-mark sentinel on the Prolog trail names %p, outside the heap reservation %p..%p -- a choice point's HB is the frontier at its creation\n",
        (const void *)hb, (const void *)g_hp_arena, (const void *)g_hp_cap_end);
    abort();
}
static void gc_index_c(void) {
    long n = 0;
    for (char *q = g_hp_arena; q < g_hp_top; q += ((rt_hblk_t *)q)->size) {
        if (n == g_gc_icap) {
            g_gc_icap = g_gc_icap ? g_gc_icap * 2 : 4096;
            g_gc_idxbuf = (rt_hblk_t **)gcbk_grow((void *)g_gc_idxbuf, (size_t)g_gc_icap * sizeof(*g_gc_idxbuf));
            if (!g_gc_idxbuf) abort();
        }
        g_gc_idxbuf[n++] = (rt_hblk_t *)q;
    }
    g_gc_idx = g_gc_idxbuf;
    g_gc_nblk = n;
}
#if RT_DIAG
static void gc_fix_check(char *base, size_t logran) {
    for (long i = 0; i < g_gc_mk.nslot; i++) {
        const char *val = g_gc_mk.slots[i].val;
        size_t g = (size_t)(val - g_hp_arena) >> 4;
        if (g < logran || !((g_hp_fr.live[g >> 6] >> (g & 63)) & 1u)) continue;
        if (*g_gc_mk.slots[i].loc != val && *g_gc_mk.slots[i].loc != base + gc_rank(val)) {
            fprintf(stderr, "[ZGC] FIX REFUSED: slot %p was written between mark and fix (holds %p, registered %p) -- a slot the collector rewrites is untouched between the two (ARCH-GC 15.11 (1))\n",
                (const void *)g_gc_mk.slots[i].loc, (const void *)*g_gc_mk.slots[i].loc, (const void *)val);
            abort();
        }
    }
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_validate_record(const char *base, const char *oldtop) {
    long n = g_gc_mk.nslot, hp = 0, ex = 0, mf = 0;
    for (long i = 0; i < n; i++) {
        const char **loc = g_gc_mk.slots[i].loc;
        const char *val = g_gc_mk.slots[i].val, *want = (val >= g_hp_arena && val < oldtop) ? base + gc_rank(val) : val, *v;
        rt_hblk_t *h;
        if ((const char *)loc >= g_hp_arena && (const char *)loc < oldtop) loc = (const char **)(uintptr_t)(base + gc_rank((const char *)loc));
        v = *loc;
        if (v < g_hp_arena || v >= g_hp_cap_end) continue;
        hp++;
        h = gc_blk_of(v);
        if (h && h->type != HB_FILL && v == want) continue;
        if (ex + mf < 16)
            fprintf(stderr, "[ZGC-VALIDATE] %s collection=%ld slot=%p value=%p want=%p registered=%p new_top=%p\n", (h && h->type != HB_FILL) ? "MISFIXED" : "EXPIRED", g_gc_runs + 1, (void *)loc,
            (void *)v, (void *)want, (void *)val, (void *)g_hp_top);
        if (h && h->type != HB_FILL) mf++;
        else ex++;
    }
    fprintf(stderr, "[ZGC-VALIDATE] collection=%ld slots=%ld heap=%ld expired=%ld misfixed=%ld\n", g_gc_runs + 1, n, hp, ex, mf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_collect_ex(const gc_ent_t *ent) {
    extern void kw_cset_gc_roots(void);
    extern void core_gc_roots(void);
    extern void dat_gc_roots(void);
    extern void gen_gc_roots(void);
    extern void pas_gc_roots(void);
    extern void pl_gc_roots(void);
    extern void rt_gc_root_args(void);
    extern void eval_gc_roots(void);
    extern void lower_gc_roots(void);
    extern void bnd_gc_roots(void);
    extern void drv_gc_roots(void);
    extern int rt_scan_active(void);
#if RT_DIAG
    char anchor;
    long words = 0, interior = 0;
    long nlive = 0, nfill = 0, before_b, after_b, n_mk = 0, n_fw = 0;
    char *dest;
    long w_cnt = 0, w_idx = 0, w_pmg = 0, w_fwd = 0, w_liv = 0, w_sld = 0, w_vfy = 0, w_cel = 0, w_raw = 0, w_mov = 0, w_unm = 0;
    int w_tel = g_gc_mk.telem;
    int st_n = 0, st_slot[32];
    uint64_t st_word[32], st_fwd[32];
    rt_hblk_t *st_blk[32];
    double n_cnt = 0, n_idx = 0, n_mrk = 0, n_fwd = 0, n_liv = 0, n_sld = 0, n_vfy = 0, n_fix = 0, n_t0 = 0, n_all = w_tel ? gc_walk_ns() : 0;
#else
    char anchor;
    long before_b, after_b;
    char *dest;
#endif
    uint32_t mxs = 0;
    long n_hb = 0, fd = 0, lt = 0;
    char *base;
    g_sxt_owner = (char *)0;
    if (g_gc_in || !g_hp_arena) return 0;
    g_gc_in = 1;
    before_b = (long)(g_hp_top - g_hp_arena);
#if RT_DIAG
    gc_quar_release(g_hp_cap_end);
    if (g_hp_flo) {
        if (mprotect(g_hp_flo, (size_t)(g_hp_fhi - g_hp_flo), PROT_READ | PROT_WRITE) != 0) {
            fprintf(stderr,
                "[GC-FLIP] could not re-open the protected from-space arena+%ld..arena+%ld for the collection -- the plant refuses the run rather than collect over ground it cannot read\n",
                (long)(g_hp_flo - g_hp_arena), (long)(g_hp_fhi - g_hp_arena));
            abort();
        }
        g_hp_flo = (char *)0;
        g_hp_fhi = (char *)0;
    }
    g_gc_flip_to = (char *)0;
    g_hp_win = (char *)0;
    g_hp_wend = (char *)0;
    n_t0 = w_tel ? gc_walk_ns() : 0;
    g_gc_nblk = 0;
    if (w_tel) { w_cnt = g_gc_nblk; n_cnt = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
#else
    g_hp_win = (char *)0;
    g_hp_wend = (char *)0;
    g_gc_nblk = 0;
#endif
    g_gc_mk.nmk = 0;
    g_gc_mk.mxs = 0;
    g_gc_mk.arena = g_hp_arena;
    g_gc_mk.top = g_hp_top;
    g_gc_mk.ssum = g_hp_fr.ssum;
    g_gc_mk.start = g_hp_fr.start;
    g_gc_mk.live = g_hp_fr.live;
    g_gc_mk.vis = g_hp_fr.vis;
    g_gc_mk.rng_p = (DESCR_t *)0;
    g_gc_mk.rng_n = 0;
    g_gc_mk.interior = g_gc_mk.dvec_elems = g_gc_mk.dvec_nondvec = g_gc_mk.walked = 0;
    g_gc_mk.mxs = 0;
    g_gc_idx = (rt_hblk_t **)0;
    rt_gc_ssum_run(&g_hp_fr, (((size_t)(g_hp_top - g_hp_arena) >> 4) + 63) >> 6);
#if RT_DIAG
    if (g_gc_mk.diag || g_gc_mk.trap || w_tel || g_gc_mk.plant) gc_index_c();
#ifdef SCRIP_GC_AUDIT_B
    if (!g_gc_idx && getenv("SCRIP_GC_AUDIT_B")) gc_index_c();
#endif
#endif
#if RT_DIAG
    if (w_tel) { w_idx = g_gc_nblk; n_idx = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    g_gc_mk.mhead = (rt_hblk_t *)0;
    g_gc_spine_recn = 0;
    g_gc_top_graph = (const char *)0;
    g_gc_mk.hn = 0;
    if (g_gc_mk.hs) memset(g_gc_mk.hs, 0, (size_t)g_gc_mk.hcap * sizeof(void *));
    g_gc_mk.nslot = 0;
    g_gc_interior = 0;
#else
    g_gc_mk.mhead = (rt_hblk_t *)0;
    g_gc_mk.hn = 0;
    if (g_gc_mk.hs) memset(g_gc_mk.hs, 0, (size_t)g_gc_mk.hcap * sizeof(void *));
    g_gc_mk.nslot = 0;
#endif
    { extern void pl_tr_gc_root_ball(const char *); for (long i = 0; i < g_gc_rrng_n; i++) if (!g_gc_rrng[i].hi) pl_tr_gc_root_ball(g_gc_rrng[i].lo); }
    g_gc_mk.draining = 1;
    {
        gc_trail_roots_args_t ta = { g_gc_rrng, g_gc_rrng_n, 0, (const char *)0, 0, (const char *)0 };
        extern long rt_gc_trail_roots_run(gc_trail_roots_args_t *);
        for (;;) { long r = rt_gc_trail_roots_run(&ta); if (!r) break; if (r == 1) gc_slot_grow(); else gc_wl_grow(); }
        n_hb = ta.n_hb;
    }
    g_gc_mk.draining = 0;
    gc_wl_drain();
#if RT_DIAG
    gc_coexpr_records();
    words += gc_stack_segments(g_gc_seam_sp ? g_gc_seam_sp : &anchor, ent);
#else
    gc_coexpr_records();
    gc_stack_segments(g_gc_seam_sp ? g_gc_seam_sp : &anchor, ent);
#endif
    g_gc_mk.draining = 0;
    gc_wl_drain();
    gc_root_cas();
    gc_root_gva();
    kw_cset_gc_roots();
    core_gc_roots();
    dat_gc_roots();
    gen_gc_roots();
    pas_gc_roots();
    pl_gc_roots();
    rt_gc_root_args();
    eval_gc_roots();
    lower_gc_roots();
    bnd_gc_roots();
    drv_gc_roots();
#if RT_DIAG
    if (g_gc_mk.maps) for (int k = 0; k < GC_REP_POPS; k++) if (g_gc_rep_ranges[k])
        fprintf(stderr, "[GC-MAPS] pop=%-7s ranges=%ld bytes=%ld agree=%ld map_only=%ld sniff_only=%ld divergence=%ld raw_hits=%ld\n", g_gc_rep_popname[k], g_gc_rep_ranges[k], g_gc_rep_bytes[k],
        g_gc_rep_agree[k], g_gc_rep_map_only[k], g_gc_rep_sniff_only[k], g_gc_rep_map_only[k] + g_gc_rep_sniff_only[k], g_gc_rep_raw[k]);
    gc_walk_print();
    if (g_gc_mk.maps) {
        fprintf(stderr, "[GC-BLOB] frames=%ld entries=%ld descr=%ld ptr_gc=%ld raw=%ld code=%ld raw_in_heap=%ld\n", g_gc_blob_frames, g_gc_blob_entries, g_gc_blob_descr, g_gc_blob_ptr, g_gc_blob_raw,
            g_gc_blob_code, g_gc_blob_raw_in_heap);
        {
            long ion = 0, ioff = 0;
            scrip_co_gc_images(&ion, &ioff);
            fprintf(stderr, "[GC-COEXPR] ctxs=%ld parked=%ld sigma=%ld images_on_stack=%ld images_off_stack=%ld\n", g_gc_co_ctxs, g_gc_co_parked, g_gc_co_sigma, ion, ioff);
        }
    }
    g_gc_blob_frames = g_gc_blob_entries = g_gc_blob_descr = g_gc_blob_ptr = g_gc_blob_raw = g_gc_blob_code = g_gc_blob_raw_in_heap = 0;
    {
        static int au = -1;
        if (au < 0) { const char *e = getenv("SCRIP_GC_AUDIT_SLOTS"); au = (e && *e && *e != '0') ? 1 : 0; }
        if (au) {
            extern void gen_gc_audit_scan_slots(long *, long *);
            long hp = 0, un = 0;
            gen_gc_audit_scan_slots(&hp, &un);
            fprintf(stderr, "[GC-AUDIT] scan-save heap=%ld unrooted=%ld\n", hp, un);
        }
    }
    if (w_tel || g_gc_mk.maps || g_gc_mk.diag) {
        for (int ci = 0; ci < 32; ci++) {
            rt_hblk_t *hb = gc_blk_of((const char *)rtccb[ci]);
            if (!hb) continue;
            g_gc_rtccb_heap++;
            if (g_gc_mk.maps)
                fprintf(stderr, "[GC-WALK-RTCCB] slot=%d word=%p graph=%s line=%ld site=%p\n", ci, (const void *)rtccb[ci], g_gc_top_graph ? g_gc_top_graph : "?", g_line, (const void *)g_gc_poll_site)
                ;
            if (st_n < 32 && (w_tel || g_gc_mk.maps)) { st_slot[st_n] = ci; st_word[st_n] = rtccb[ci]; st_blk[st_n] = hb; st_fwd[st_n] = 0; st_n++; }
        }
    }
#endif
    if (g_gc_shield_r && *g_gc_shield_r) {
        extern const char *Σ;
        extern const char *g_scan_subj;
        if (*g_gc_shield_r == Σ || *g_gc_shield_r == g_scan_subj) rt_gc_visit_raw(g_gc_shield_r);
        else if (!g_gc_shield_r_is_probe) {
            fprintf(stderr,
                "[ZHP] rt_gc_point_arr_c refuses r0 %p: a CALLER handed this shield a word to protect and it aliases neither the subject nor g_scan_subj, so this shield cannot reach it and honouring"
                " t"
                "he call silently would drop a root -- pass the word as a tagged DESCR cell in arr[] instead (CEO-972). The asm shim's saved-subject PROBE is rt_gc_point_arr_probe_c and is not this "
                "road.\n", (const void *)*g_gc_shield_r);
            abort();
        }
    }
    { extern const char *Σ; rt_gc_visit_raw(&Σ); }
    for (int si = 0; si < g_gc_shield_n; si++) rt_gc_visit_descr(&g_gc_shield_arr[si]);
    {
#if RT_DIAG
        long rounds = 0;
#endif
        {
            for (;;) {
                if (!g_gc_mk.mhead && !g_gc_mk.wln) break;
#if RT_DIAG
                rounds++;
#endif
                g_gc_mk.draining = 1;
                gc_wl_drain();
                g_gc_mk.draining = 0;
            }
        }
#if RT_DIAG
        if (w_tel) {
            n_mrk = gc_walk_ns() - n_t0;
            n_t0 = gc_walk_ns();
            fprintf(stderr, "[ZGC-MARK] arm=%s titles-walked=%ld blocks-scanned=%ld rounds=%ld nblk=%ld\n", "WL", g_gc_mk.walked, g_gc_mk.walked, rounds, g_gc_nblk);
            n_t0 = gc_walk_ns();
        }
#endif
    }
#if RT_DIAG
    g_gc_interior += g_gc_mk.interior;
    g_gc_dvec_elems += g_gc_mk.dvec_elems;
    g_gc_dvec_nondvec += g_gc_mk.dvec_nondvec;
#endif
    { extern void kw_cset_gc_weak(void); kw_cset_gc_weak(); }
#ifdef SCRIP_GC_AUDIT_B
#if RT_DIAG
    gc_audit_b_shim(g_gc_seam_sp ? g_gc_seam_sp : &anchor);
#endif
#endif
#if RT_DIAG
    gc_spine_lost_check();
    if (g_gc_mk.diag) gc_assert_check();
    {
        static int cov = -1;
        if (cov < 0) { const char *e = getenv("SCRIP_GC_COVERAGE"); cov = (e && *e && *e != '0') ? 1 : 0; }
        if (cov)
            fprintf(stderr, "[GC-COV] ranges=%ld cas_scanned_bytes=%ld words_scanned=%ld interior_words=%ld program_top=%p\n", g_gc_rrng_n, g_gc_cas_bytes, words, interior, (void *)g_gc_program_top);
    }
#endif
    { size_t topg = (size_t)(g_hp_top - g_hp_arena) >> 4; fd = rt_gc_first_dead_run(g_hp_fr.live, topg); lt = (long)rt_gc_prefix_run(g_hp_fr.prefix, g_hp_fr.live, (topg + 63) >> 6); }
    base = g_hp_arena;
#if RT_DIAG
    if (g_gc_mk.plant) {
        size_t pg = gc_pg();
        char *up = (char *)(((uintptr_t)g_hp_top + pg - 1) & ~(uintptr_t)(pg - 1));
        size_t fw = 0, tw = (((size_t)(g_hp_top - g_hp_arena) >> 4) + 63) >> 6;
        while (fw < tw && !g_hp_fr.live[fw]) fw++;
        if (up + lt <= g_hp_end && (size_t)(up - g_hp_arena) < (size_t)0xFFFFFFF0u) g_gc_flip_to = up;
        else if (fw >= tw || g_hp_arena + lt <= g_hp_arena + (((fw << 6) + (size_t)__builtin_ctzll(g_hp_fr.live[fw])) << 4)) g_gc_flip_to = g_hp_arena;
        else {
            static int said_flip = 0;
            g_gc_flip_declined++;
            if (!said_flip) {
                said_flip = 1;
                fprintf(stderr,
                    "[GC-FLIP] plant DECLINED at a collection: %ld live bytes fit neither above the page-rounded top (arena+%ld, committed end arena+%ld) nor below the first live block (arena+%l"
                    "d), so THAT collection compacted in place and its stale copies were not trapped; the count is flip_declined= on the GC-EXERCISE line. Printed ONCE per process.\n", lt,
                    (long)(up - g_hp_arena), (long)(g_hp_end - g_hp_arena), fw < tw ? (long)(((fw << 6) + (size_t)__builtin_ctzll(g_hp_fr.live[fw])) << 4) : 0L);
            }
        }
        if (g_gc_flip_to) { base = g_gc_flip_to; g_gc_flip_live = lt; }
    }
    nlive = g_gc_mk.nmk;
    n_mk = n_fw = g_gc_mk.nmk;
    if (w_tel) { w_fwd = g_gc_nblk; n_fwd = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    gc_vac_record(base, g_gc_flip_to ? g_hp_arena : g_hp_arena + lt);
#endif
#if RT_DIAG
    for (int z = 0; z < st_n; z++) st_fwd[z] = gc_blk_marked(st_blk[z]) ? (uint64_t)(uintptr_t)(base + gc_rank((const char *)st_blk[z])) : 0;
#endif
#if RT_DIAG
    if (g_gc_mk.diag) gc_fix_check(base, base == g_hp_arena ? (size_t)fd : 0);
#endif
    rt_gc_fix_run(g_gc_mk.slots, g_gc_mk.nslot, base, base == g_hp_arena ? (size_t)fd : 0, &g_hp_fr, g_hp_arena);
#if RT_DIAG
    if (w_tel) { w_cel = g_gc_mk.nslot; w_raw = 0; n_fix = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
    if (w_tel) n_t0 = gc_walk_ns();
#endif
    {
        char *oldtop = g_hp_top;
#if RT_DIAG
        char *d0 = base + ((base == g_hp_arena ? (size_t)fd : 0) << 4);
        if (w_tel && base == g_hp_arena) for (char *q = g_hp_arena; q < d0; q += ((rt_hblk_t *)q)->size) w_unm++;
#endif
        dest = rt_gc_slide_run(base, fd, &g_hp_fr, g_hp_arena, oldtop);
        rt_gc_start_compact_run(&g_hp_fr, g_hp_arena, base, oldtop);
        rt_gc_ssum_run(&g_hp_fr, (((size_t)((dest > oldtop ? dest : oldtop) - g_hp_arena) >> 4) + 63) >> 6);
        mxs = (uint32_t)g_gc_mk.mxs;
#if RT_DIAG
        if (w_tel) w_mov += (long)(dest - d0);
        if (g_gc_flip_to) {
            static int said_applied = 0;
            if (base > g_hp_arena) {
                rt_hblk_t *fl = (rt_hblk_t *)g_hp_arena;
                fl->fwd = 0;
                fl->size = (uint32_t)(base - g_hp_arena);
                fl->type = HB_FILL;
                fl->flags = HBF_TTL;
                gc_start_set(g_hp_arena);
                nfill++;
            }
            g_gc_flips++;
            if (w_tel || g_gc_mk.maps || !said_applied) {
                said_applied = 1;
                fprintf(stderr,
                    "[GC-FLIP] plant: every live block copied to ground disjoint from its old address (%ld live bytes to arena+%ld) and ALL of the old ground poisoned and PROT_NONE until the next co"
                    "llection, so a stale copy of ANY heap address faults at the instruction that uses it. THIS LINE IS THE ONLY PROOF THE PLANT APPLIED, so it prints ONCE PER PROCESS with nothing a"
                    "sked for.\n", g_gc_flip_live, (long)(g_gc_flip_to - g_hp_arena));
            }
        }
#endif
    }
#if RT_DIAG
    g_hp_top = dest;
    g_hp_blocks = nlive + nfill;
    g_hp_live = g_gc_flip_to ? g_gc_flip_live : (long)(dest - g_hp_arena);
    if (g_gc_flip_to && g_hp_top > g_hp_virgin) g_hp_virgin = g_hp_top;
    rt_gcheap_line_reset((long)mxs);
#else
    g_hp_top = dest;
    g_hp_live = (long)(dest - g_hp_arena);
    rt_gcheap_line_reset((long)mxs);
#endif
    { static int vld = -1; if (vld < 0) { const char *e = getenv("SCRIP_GC_VALIDATE"); vld = (e && *e == '1') ? 1 : 0; } if (vld) gc_validate_record(base, g_hp_arena + before_b); }
    if (n_hb) {
        gc_tr_args_t ta = { g_gc_rrng, g_gc_rrng_n, base, g_hp_arena + before_b, g_hp_top, &g_hp_fr, g_hp_arena, g_hp_cap_end };
        uint64_t bad = rt_gc_trail_run(&ta);
        if (bad) gc_trail_refuse((const char *)(uintptr_t)bad);
    }
    rt_gc_clear_run(&g_hp_fr, (((size_t)before_b >> 4) + 63) >> 6);
    after_b = (long)(g_hp_top - g_hp_arena);
#if RT_DIAG
    if (st_n) {
        static int said_stale = 0;
        long mv = 0, un = 0, sw = 0;
        for (int z = 0; z < st_n; z++) {
            int moved = (st_fwd[z] && st_fwd[z] != (uint64_t)(uintptr_t)st_blk[z]) ? 1 : 0, kept = (rtccb[st_slot[z]] == st_word[z]) ? 1 : 0;
            if (moved) mv++;
            if (moved && kept) un++;
            if (!st_fwd[z] && kept) sw++;
            if (g_gc_mk.maps)
                fprintf(stderr, "[GC-RTCCB-STALE] slot=%d word=%p block=type%d/%ldB forwarded=%d moved=%d unrepaired=%d swept=%d graph=%s line=%ld site=%p\n", st_slot[z],
                (const void *)(uintptr_t)st_word[z], (int)st_blk[z]->type, (long)st_blk[z]->size, st_fwd[z] ? 1 : 0, moved, (moved && kept) ? 1 : 0, (!st_fwd[z] && kept) ? 1 : 0,
                g_gc_top_graph ? g_gc_top_graph : "?", g_line, (const void *)g_gc_poll_site);
        }
        if ((un || sw) && (w_tel || g_gc_mk.maps || !said_stale)) {
            said_stale = 1;
            fprintf(stderr,
                "[GC-RTCCB-STALE] SUMMARY collection=%ld slots-holding-a-heap-pointer=%d moved=%ld UNREPAIRED=%ld SWEPT=%ld. This is the STANDING population and not a plant: these are words the emit"
                "ter itself spilled into the caller-saved block before a safe point (x86_asm.h 386-389), and the block is registered as a GC root by rtcc_init.c while the collector's only walk over "
                "the range table skips exactly that shape, so nothing visits them. UNREPAIRED means the collector FORWARDED the block the word names and left the word holding the pre-move address, w"
                "hich the emitted reload after the poll then restores into a register; SWEPT means the block was not forwarded at all and the word names ground the arena will re-issue. A pointer in "
                "an unvisited slot is necessary but not sufficient for a lost value -- the same block may be rooted elsewhere, which is how these witnesses still answer their oracles -- but UNREPAIR"
                "ED and SWEPT are the two ways it stops being merely necessary, and they are counted here rather than argued.\n", g_gc_runs + 1, st_n, mv, un, sw);
        }
    }
    {
        static int psn = -1;
        if (psn < 0) { const char *e = getenv("SCRIP_GC_POISON"); psn = (e && *e) ? (*e != '0') : g_gc_mk.diag; }
        if (psn) {
            char *ot = g_hp_arena + before_b;
            long pb = ot > g_hp_top ? (long)(ot - g_hp_top) : 0;
            if (pb > 0) memset(g_hp_top, 0xDB, (size_t)pb);
            if (w_tel) fprintf(stderr, "[ZGC-POISON] vacated=%ldB filled=0xDB top=%p oldtop=%p\n", pb, (void *)g_hp_top, (void *)ot);
        }
    }
    if (w_tel) { w_sld = g_gc_mk.nmk; n_sld = gc_walk_ns() - n_t0; n_t0 = gc_walk_ns(); }
#endif
    { extern void rt_nv_memo_invalidate(void); extern void pm_lf_type_invalidate(void); rt_nv_memo_invalidate(); pm_lf_type_invalidate(); }
#if RT_DIAG
    if (g_gc_mk.verify) { rt_gcheap_verify(); if (w_tel) { w_vfy = nlive + nfill; n_vfy = gc_walk_ns() - n_t0; } }
#endif
    g_gc_runs++;
#if RT_DIAG
    if (w_tel)
        fprintf(stderr,
        "[ZGC-WALK] arm=%s nblk=%ld | count=%ld/%.0fus index=%ld/%.0fus pmap-gran=%ld fwd=%ld/%.0fus live=%ld/%.0fus | mark=%.0fus fixup=%ld+%ld/%.0fus slide=%ld/%.0fus moved=%ldB unmoved=%ld verify"
        "=%ld/%.0fus | walk-floor=%ld titles %.0fus of %.0fus total\n", "FOLD", g_gc_nblk, w_cnt, n_cnt / 1e3, w_idx, n_idx / 1e3, w_pmg, w_fwd, n_fwd / 1e3, w_liv, n_liv / 1e3, n_mrk / 1e3, w_cel,
        w_raw, n_fix / 1e3, w_sld, n_sld / 1e3, w_mov, w_unm, w_vfy, n_vfy / 1e3, w_cnt + w_idx + w_fwd + w_liv + w_vfy, (n_cnt + n_idx + n_fwd + n_liv + n_vfy) / 1e3, (gc_walk_ns() - n_all) / 1e3);
    if (g_gc_dvec_elems && (w_tel || g_gc_mk.maps)) fprintf(stderr, "[GC-DVEC] elems=%ld non_dvec=%ld\n", g_gc_dvec_elems, g_gc_dvec_nondvec);
    if (w_tel)
        fprintf(stderr, "[ZGC] regeneration #%ld (%s): blocks %ld->%ld (fill %ld) bytes %ld->%ld reclaimed %ld win=%ld slots=%ld interior=%ld wl_depth_max=%ld marked=%ld forwarded=%ld\n", g_gc_runs,
        "E", g_gc_nblk, nlive, nfill, before_b, after_b, before_b - after_b, (long)(g_hp_wend - g_hp_win), g_gc_mk.nslot, g_gc_interior, g_gc_mk.wlmax, n_mk, n_fw);
#endif
    if (g_hp_cap_end > g_hp_end && (long)(g_hp_end - g_hp_arena - g_hp_live) * 100L < (long)(g_hp_end - g_hp_arena) * (long)GC_FREE_PCT) (void)rt_gcheap_grow(0);
    rt_gcheap_line_reset((long)mxs);
#if RT_DIAG
    if (g_gc_flip_to && g_gc_flip_to > g_hp_arena + sizeof(rt_hblk_t)) {
        size_t pg = gc_pg();
        char *a = (char *)(((uintptr_t)g_hp_arena + sizeof(rt_hblk_t) + pg - 1) & ~(uintptr_t)(pg - 1)), *b = (char *)((uintptr_t)g_gc_flip_to & ~(uintptr_t)(pg - 1));
        memset(g_hp_arena + sizeof(rt_hblk_t), 0xDB, (size_t)(g_gc_flip_to - g_hp_arena - (long)sizeof(rt_hblk_t)));
        if (b > a && mprotect(a, (size_t)(b - a), PROT_NONE) == 0) { g_hp_flo = a; g_hp_fhi = b; }
    }
    gc_quar_arm(g_hp_top);
    if (w_tel)
        fprintf(stderr, "[ZGC-TRAP] quarantine arena+%ld..arena+%ld (%ld pages PROT_NONE) ledger=%ld entries over=%ld arms=%ld\n", g_hp_qlo ? (long)(g_hp_qlo - g_hp_arena) : -1L,
        g_hp_qhi ? (long)(g_hp_qhi - g_hp_arena) : -1L, g_hp_qlo ? (long)((g_hp_qhi - g_hp_qlo) / (long)gc_pg()) : 0L, g_gc_vacn, g_gc_vacover, g_hp_qarm);
#endif
    g_gc_idx = (rt_hblk_t **)0;
    g_gc_in = 0;
    return before_b - after_b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_collect_c(char *floor) { long r; g_gc_seam_sp = floor; r = gc_collect_ex((const gc_ent_t *)0); g_gc_seam_sp = (char *)0; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_poll(void) {
    if (g_gc_in) return;
    if (!g_gc_pending && !(g_hp_gcline && g_hp_top > g_hp_gcline)) return;
#if RT_DIAG
    g_gc_polls++;
    g_gc_poll_site = (const char *)__builtin_return_address(0);
#endif
    gc_ent_t ent = { __builtin_return_address(0), (const char *)__builtin_frame_address(0) + 16, *(const void *const *)__builtin_frame_address(0) };
    gc_point_arr_run((DESCR_t *)0, 0, (const char **)0, (char *)__builtin_frame_address(0) + 16, 0, &ent);
}
_Static_assert(sizeof(void *) == 8,
    "EVERY POLL ENTRY HANDS THE COLLECTOR THE SAME THREE FACTS (cto, ARCH-GC section 13.4): the floor R is the emitted code's rsp at the call instruction (rt_gc_poll: frame address + 16; rt_gc_poll_"
    "slow: rsp + 96 above its nine pushes, the r13 word and the return address), the return PC into emitted code is the word under it at R - 8, and the emitted rbp is the saved rbp of the first C fr"
    "ame or the untouched rbp of the asm leaf -- the chain over the maps starts from these, nothing is sniffed. The old text: rt_gc_poll's floor is its caller's stack pointer: the frame pointer plus"
    " the saved rbp and the return address, 16 bytes on x86-64 -- the walk steps 8-byte words from this floor and never aligns it, so a floor taken from a char local (an odd address) misreads every "
    "cell above it (cto 2026-09-23, user_function_opsyn_8 printed empty at the association tap's spilled-pair poll)");
#if RT_DIAG
long rt_gc_polls_count(void) { return g_gc_polls; }
#else
long rt_gc_polls_count(void) { return 0; }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_poll_slow(void);
void rt_gc_poll_asm(void);
__asm__( ".text\n" ".globl rt_gc_poll_slow\n" ".type rt_gc_poll_slow,@function\n" "rt_gc_poll_slow:\n" "  .cfi_startproc\n" "  pushq %rax ; .cfi_adjust_cfa_offset 8\n"
    "  pushq %rcx ; .cfi_adjust_cfa_offset 8\n" "  pushq %rdx ; .cfi_adjust_cfa_offset 8\n" "  pushq %rsi ; .cfi_adjust_cfa_offset 8\n" "  pushq %rdi ; .cfi_adjust_cfa_offset 8\n"
    "  pushq %r8 ; .cfi_adjust_cfa_offset 8\n" "  pushq %r9 ; .cfi_adjust_cfa_offset 8\n" "  pushq %r10 ; .cfi_adjust_cfa_offset 8\n" "  pushq %r11 ; .cfi_adjust_cfa_offset 8\n"
    "  subq  $16, %rsp ; .cfi_adjust_cfa_offset 16\n" "  movq  %r13, (%rsp)\n"
#if RT_DIAG
"  incq g_gc_polls(%rip)\n" "  movq 88(%rsp), %rax\n" "  movq %rax, g_gc_poll_site(%rip)\n"
#endif
"  xorl %edi, %edi\n" "  xorl %esi, %esi\n" "  movq %rsp, %rdx\n" "  leaq 88(%rsp), %rcx\n" "  movl $1, %r8d\n" "  call gc_point_arr_body\n" "  movq (%rsp), %r13\n"
    "  addq  $16, %rsp ; .cfi_adjust_cfa_offset -16\n" "  popq %r11 ; .cfi_adjust_cfa_offset -8\n" "  popq %r10 ; .cfi_adjust_cfa_offset -8\n" "  popq %r9 ; .cfi_adjust_cfa_offset -8\n"
    "  popq %r8 ; .cfi_adjust_cfa_offset -8\n" "  popq %rdi ; .cfi_adjust_cfa_offset -8\n" "  popq %rsi ; .cfi_adjust_cfa_offset -8\n" "  popq %rdx ; .cfi_adjust_cfa_offset -8\n"
    "  popq %rcx ; .cfi_adjust_cfa_offset -8\n" "  popq %rax ; .cfi_adjust_cfa_offset -8\n" "  ret\n" "  .cfi_endproc\n" ".size rt_gc_poll_slow,.-rt_gc_poll_slow\n" );
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__asm__( ".text\n" ".globl rt_gc_poll_asm\n" ".type rt_gc_poll_asm,@function\n" "rt_gc_poll_asm:\n" "  .cfi_startproc\n" "  cmpl $0, g_gc_in(%rip)\n" "  jne  9f\n"
    "  pushq %r11 ; .cfi_adjust_cfa_offset 8\n" "  movq g_gc_pending@GOTPCREL(%rip), %r11\n" "  cmpl $0, (%r11)\n" "  jne  2f\n" "  movq g_hp_gcline(%rip), %r11\n" "  testq %r11, %r11\n" "  je   1f\n"
    "  pushq %r10 ; .cfi_adjust_cfa_offset 8\n" "  movq g_hp_fr@GOTPCREL(%rip), %r10\n" "  movq (%r10), %r10\n" "  cmpq %r11, %r10\n" "  popq %r10 ; .cfi_adjust_cfa_offset -8\n" "  jbe  1f\n" "2:\n"
    "  popq %r11 ; .cfi_adjust_cfa_offset -8\n" "  jmp  rt_gc_poll_slow@PLT\n" "1:\n" "  .cfi_def_cfa_offset 16\n" "  popq %r11 ; .cfi_adjust_cfa_offset -8\n" "9:\n" "  ret\n" "  .cfi_endproc\n"
    ".size rt_gc_poll_asm,.-rt_gc_poll_asm\n" );
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gcheap_free(void) { if (!g_hp_arena) rt_gcheap_init(); return (long)(g_hp_end - g_hp_top); }
long rt_gcheap_stat(int which) {
    if (!g_hp_arena) rt_gcheap_init();
    if (which == 0) return (long)g_hp_chunk;
    if (which == 1) return (long)(g_hp_cap_end - g_hp_arena);
    if (which == 2) return (long)(intptr_t)g_hp_arena;
    if (which == 3) return (long)(intptr_t)g_hp_end;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
long rt_gc_cb_open(void) { return gc_diag_on() ? rt_gc_runs_count() : -1L; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_gc_cb_close(long mark, const char *file, int line, void *lo, void *hi) {
    long n = 0;
    if (rt_gc_runs_count() == mark) return 0;
    if (!g_hp_arena || !lo || !hi || (char *)hi <= (char *)lo) return 0;
    { char **w = (char **)((((uintptr_t)lo) + 7u) & ~(uintptr_t)7u); for (; (char *)(w + 1) <= (char *)hi; w++) { char *v = *w; if (v > g_hp_arena && v < g_hp_top) n++; } }
    if (n) {
        static int loud = -1;
        if (loud < 0) { const char *e = getenv("SCRIP_GC_CB_REPORT"); loud = (e && *e != '0') ? 1 : 0; }
        if (loud)
            fprintf(stderr,
            "[ZGC-CB] %s:%d %ld raw arena word(s) live in an UNMAPPED C FRAME across a callback during which the collector ran %ld time(s). Lon 2026-09-21: a C function creates residue on the hardwa"
            "re stack; no compile-time frame map describes this frame, so these words were not updated and the collector could not have updated them without guessing (ARCH-GC section 7, section 9(ii"
            ") as reversed at CEO-1090).\n", file ? file : "?", line, n, rt_gc_runs_count() - mark);
        {
            static int fatal = -1;
            if (fatal < 0) { const char *e = getenv("SCRIP_GC_CB_FATAL"); fatal = (e && *e != '0') ? 1 : 0; }
            if (fatal) { fprintf(stderr, "[ZGC-CB] SCRIP_GC_CB_FATAL is set: aborting on the first Rule 4 violation.\n"); abort(); }
        }
    }
    return n;
}
#else
long rt_gc_cb_open(void) { return 0; }
long rt_gc_cb_close(long mark, const char *file, int line, void *lo, void *hi) { (void)mark; (void)file; (void)line; (void)lo; (void)hi; return 0; }
#endif
long rt_gc_runs_count(void) { return g_gc_runs; }
long rt_gc_heap_grows(void) { return g_hp_grows; }
#if RT_DIAG
void rt_gc_assert_dead(const void *payload) { if (!payload || (const char *)payload <= g_hp_arena || (const char *)payload >= g_hp_top) return; ((rt_hblk_t *)payload - 1)->flags |= HBF_ASSERT_DEAD; }
void *rt_gc_assert_instances(uint16_t type, long n) {
    DESCR_t *v = (DESCR_t *)rt_gcheap_alloc(HB_DVEC, 2 * sizeof(DESCR_t));
    v[0] = INTVAL((long long)type);
    v[1] = INTVAL((long long)n);
    ((rt_hblk_t *)v - 1)->flags |= HBF_ASSERT_INST;
    return (void *)v;
}
#else
void rt_gc_assert_dead(const void *payload) { (void)payload; }
void *rt_gc_assert_instances(uint16_t type, long n) { (void)type; (void)n; return (void *)0; }
#endif
