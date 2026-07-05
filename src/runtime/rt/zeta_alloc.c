#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <gc.h>
#include "zeta_choices.h"
#include "zeta_alloc.h"
#define ZLS_HDR 16L
#define ZLS_ROOT_CHUNK (8L * 1024 * 1024)
static char *g_zls_arena = (char *)0;
static char *g_zls_top = (char *)0;
static char *g_zls_end = (char *)0;
static char *g_zls_hiwater = (char *)0;
static char *g_zls_rooted = (char *)0;
static void *g_zls_cur = (void *)0;
static long  g_zls_allocs = 0;
static long  g_zls_releases = 0;
static int   g_zls_report_reg = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_report(void)
{
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    fprintf(stderr, "[ZLS] ZC_ALLOC=%d ZC_INIT=%d ZC_POISON=%d arena=%dMB hiwater=%ldB allocs=%ld releases=%ld live=%ld\n", (int)ZC_ALLOC, (int)ZC_INIT, (int)ZC_POISON, (int)ZC_ARENA_MB,
            g_zls_arena ? (long)(g_zls_hiwater - g_zls_arena) : 0L, g_zls_allocs, g_zls_releases, g_zls_allocs - g_zls_releases);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_arena_init(void)
{
    long mb = (long)ZC_ARENA_MB;
    g_zls_arena = (char *)mmap((void *)0, (size_t)mb << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (g_zls_arena == MAP_FAILED) { fprintf(stderr, "[ZLS] zeta arena mmap failed (%ld MB) — lower ZC_ARENA_MB\n", mb); abort(); }
    g_zls_top = g_zls_arena; g_zls_end = g_zls_arena + ((size_t)mb << 20); g_zls_hiwater = g_zls_arena; g_zls_rooted = g_zls_arena;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_alloc(long bytes)
{
    long sz = (bytes + 15L) & ~15L;
    char *base;
    if (!g_zls_report_reg) { g_zls_report_reg = 1; atexit(rt_zls_report); }
#if ZC_ALLOC == ZC_ALLOC_MALLOC
    base = (char *)malloc((size_t)(ZLS_HDR + sz));
    if (!base) { fprintf(stderr, "[ZLS] malloc-mode activation alloc failed (%ld bytes)\n", ZLS_HDR + sz); abort(); }
    GC_add_roots(base, base + ZLS_HDR + sz);
#else
    if (!g_zls_arena) rt_zls_arena_init();
    base = g_zls_top;
    if (base + ZLS_HDR + sz > g_zls_end) { fprintf(stderr, "[ZLS] zeta arena exhausted (%d MB reserve, hiwater %ldB) — raise ZC_ARENA_MB\n", (int)ZC_ARENA_MB, (long)(g_zls_hiwater - g_zls_arena)); abort(); }
    g_zls_top = base + ZLS_HDR + sz;
    if (g_zls_top > g_zls_hiwater) {
        g_zls_hiwater = g_zls_top;
        while (g_zls_rooted < g_zls_hiwater) { char *nx = g_zls_rooted + ZLS_ROOT_CHUNK; if (nx > g_zls_end) nx = g_zls_end; GC_add_roots(g_zls_rooted, nx); g_zls_rooted = nx; }
    }
#endif
#if ZC_INIT == ZC_INIT_ZERO
    memset(base, 0, (size_t)(ZLS_HDR + sz));
#elif ZC_POISON == ZC_POISON_FILL
    memset(base, 0xAA, (size_t)(ZLS_HDR + sz));
#endif
    ((void **)base)[0] = g_zls_cur;
    ((long *)base)[1] = sz;
    g_zls_cur = (void *)(base + ZLS_HDR);
    g_zls_allocs += 1;
    return (void *)(base + ZLS_HDR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls_release(void *fb)
{
    char *base;
    if (!fb) return;
    base = (char *)fb - ZLS_HDR;
    g_zls_releases += 1;
    g_zls_cur = ((void **)base)[0];
#if ZC_ALLOC == ZC_ALLOC_MALLOC
    { long sz = ((long *)base)[1]; GC_remove_roots(base, base + ZLS_HDR + sz); }
    free(base);
#elif ZC_ALLOC == ZC_ALLOC_BUMP_LIFO
    if (base >= g_zls_arena && base < g_zls_top) {
#if ZC_POISON == ZC_POISON_FILL
        memset(base, 0xDD, (size_t)(g_zls_top - base));
#endif
        g_zls_top = base;
    }
#endif
}
