#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <gc.h>
#include "zeta_choices.h"
#include "gc_heap.h"
#include "descr.h"
/* GC-0 (ARCH-ZETA-LOCAL-STORAGE §6e) — scrip-owned bump heap, SIL title-word headers, libgc COEXISTENCE.
 * Coexistence contract (why this is sound with zero collector): scrip-heap blocks are not libgc objects, so
 * libgc never frees them; a libgc conservative scan that sees a pointer INTO this arena ignores it (non-heap);
 * and the ONLY family migrated here (DT_S string payloads) is ATOMIC — no pointers inside — so a scrip-heap
 * block can never be the sole holder of a libgc object's liveness. Families whose payloads can hold libgc
 * pointers (ARBLK/TBBLK/DATINST/VCELL DESCR cells) must NOT migrate before their GC-5 row lands with either
 * full residency or GC_add_roots registration. Nothing here frees, so dangling is impossible; garbage simply
 * accumulates until GC-1..3 land — the SIL way: bump until exhaustion, then storage regeneration (§6a). */
_Static_assert(sizeof(rt_hblk_t) == 16, "rt_hblk_t must be one 16-byte title unit");
static char *g_hp_arena = (char *)0;
static char *g_hp_top = (char *)0;
static char *g_hp_end = (char *)0;
static long  g_hp_blocks = 0;
static int   g_hp_report_reg = 0;
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
    fprintf(stderr, "[ZHP] ZC_HEAP_STRINGS=%d arena=%dMB blocks=%ld(alloc'd)=%ld(walked) bytes=%ld verify=OK\n", (int)ZC_HEAP_STRINGS, (int)ZC_HEAP_MB, g_hp_blocks, live, g_hp_arena ? (long)(g_hp_top - g_hp_arena) : 0L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gcheap_init(void)
{
    long mb = (long)ZC_HEAP_MB;
    g_hp_arena = (char *)mmap((void *)0, (size_t)mb << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (g_hp_arena == MAP_FAILED) { fprintf(stderr, "[ZHP] heap arena mmap failed (%ld MB) — lower ZC_HEAP_MB\n", mb); abort(); }
    g_hp_top = g_hp_arena; g_hp_end = g_hp_arena + ((size_t)mb << 20);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes)
{
    uint64_t total = sizeof(rt_hblk_t) + ((payload_bytes + 15u) & ~15ull);
    rt_hblk_t *h;
    if (!g_hp_report_reg) { g_hp_report_reg = 1; atexit(rt_gcheap_report); }
    if (!g_hp_arena) rt_gcheap_init();
    if (g_hp_top + total > g_hp_end) { fprintf(stderr, "[ZHP] heap exhausted (%d MB, %ld blocks live+garbage) — storage regeneration (GC-1..3) not yet landed; raise ZC_HEAP_MB or build with -DZC_HEAP_STRINGS=0\n", (int)ZC_HEAP_MB, g_hp_blocks); abort(); }
    h = (rt_hblk_t *)g_hp_top;
    g_hp_top += total;
    h->fwd = 0; h->size = (uint32_t)total; h->type = type; h->flags = HBF_TTL;
    memset((void *)(h + 1), 0, (size_t)(total - sizeof(rt_hblk_t)));
    g_hp_blocks += 1;
    return (void *)(h + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_str_alloc(long n)
{
    /* THE DT_S entry point (GC-5 strings row, landed with GC-0 as the Lon-directed proof family):
     * n characters + NUL. Zero-initialized on BOTH paths — manual pin 3's "all words within a block must be
     * properly filled in", discharged mechanically the ZC_INIT_ZERO way. Fallback = libgc atomic, intact. */
    long want = (n < 0 ? 0 : n) + 1;
#if ZC_HEAP_STRINGS == ZC_HEAP_SCRIP
    _Static_assert(DT_S == 1, "value-world heap types carry DTYPE_t verbatim");
    return (char *)rt_gcheap_alloc((uint16_t)DT_S, (uint64_t)want);
#else
    { char *b = (char *)GC_MALLOC_ATOMIC((size_t)want); if (b) memset(b, 0, (size_t)want); return b; }
#endif
}
