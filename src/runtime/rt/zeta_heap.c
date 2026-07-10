#include "zeta_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <gc.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZH_HDR 16L
#define ZH_LIVE 1
#define ZH_DEAD 2
#define ZH_POISON 0x5A
#define ZH_TAB_INIT 256
typedef struct { uint32_t total; uint32_t birth; uint32_t handle; uint16_t state; uint16_t pin; } zh_hdr_t;
static char *g_zh_base = 0; static char *g_zh_cur = 0; static long g_zh_cap = 0;
static char **g_zh_tab = 0; static unsigned g_zh_tab_cap = 0; static unsigned g_zh_tab_next = 1; static unsigned *g_zh_free = 0; static unsigned g_zh_free_n = 0; static unsigned g_zh_free_cap = 0;
static uint32_t g_zh_birth = 0; static long g_zh_allocs = 0, g_zh_deads = 0, g_zh_slides = 0, g_zh_slid_bytes = 0, g_zh_pin_holes = 0; static int g_zh_telem = -1, g_zh_atexit = 0;
static pthread_mutex_t g_zh_mu = PTHREAD_MUTEX_INITIALIZER;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zh_telem(void) { if (g_zh_telem < 0) g_zh_telem = getenv("SCRIP_ZH_TELEM") ? 1 : 0; return g_zh_telem; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zh_report(void)
{
    long live = rt_zh_live_count();
    fprintf(stderr, "[ZH] allocs=%ld deads=%ld slides=%ld slid_bytes=%ld pin_holes=%ld live_at_exit=%ld used=%ld/%ld\n", g_zh_allocs, g_zh_deads, g_zh_slides, g_zh_slid_bytes, g_zh_pin_holes, live, (long)(g_zh_cur - g_zh_base), g_zh_cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zh_init(void)
{
    long mb = 32; const char *e = getenv("SCRIP_ZH_MB"); if (e && atol(e) > 0) mb = atol(e);
    g_zh_cap = mb * 1024L * 1024L;
    g_zh_base = (char *)GC_MALLOC_UNCOLLECTABLE((size_t)g_zh_cap);
    if (!g_zh_base) { fprintf(stderr, "[ZH] FATAL: slab alloc failed (%ld MB)\n", mb); abort(); }
    g_zh_cur = g_zh_base;
    g_zh_tab_cap = ZH_TAB_INIT; g_zh_tab = (char **)GC_MALLOC_UNCOLLECTABLE(sizeof(char *) * g_zh_tab_cap); if (!g_zh_tab) abort();
    if (!g_zh_atexit) { g_zh_atexit = 1; if (zh_telem()) atexit(zh_report); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned zh_handle_get(void)
{
    if (g_zh_free_n) return g_zh_free[--g_zh_free_n];
    if (g_zh_tab_next >= g_zh_tab_cap) { unsigned nc = g_zh_tab_cap * 2; char **nt = (char **)GC_MALLOC_UNCOLLECTABLE(sizeof(char *) * nc); if (!nt) abort(); memcpy(nt, g_zh_tab, sizeof(char *) * g_zh_tab_cap); GC_FREE(g_zh_tab); g_zh_tab = nt; g_zh_tab_cap = nc; }
    return g_zh_tab_next++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zh_handle_put(unsigned h)
{
    if (g_zh_free_n >= g_zh_free_cap) { unsigned nc = g_zh_free_cap ? g_zh_free_cap * 2 : ZH_TAB_INIT; unsigned *nf = (unsigned *)GC_MALLOC_UNCOLLECTABLE(sizeof(unsigned) * nc); if (!nf) abort(); if (g_zh_free) { memcpy(nf, g_zh_free, sizeof(unsigned) * g_zh_free_n); GC_FREE(g_zh_free); } g_zh_free = nf; g_zh_free_cap = nc; }
    g_zh_tab[h] = 0; g_zh_free[g_zh_free_n++] = h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zh_nomove(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_ZH_NOMOVE"); p = e ? (atoi(e) != 0) : 1; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zh_slide(void)
{
    if (zh_nomove()) {
        char *src = g_zh_base, *last_live_end = g_zh_base;
        while (src < g_zh_cur) {
            zh_hdr_t *hd = (zh_hdr_t *)src; long tot = (long)hd->total;
            if (hd->state == ZH_LIVE) last_live_end = src + tot;
            else { if (hd->handle) { zh_handle_put(hd->handle); hd->handle = 0; } g_zh_deads++; }
            src += tot;
        }
        if (last_live_end < g_zh_cur) memset(last_live_end, ZH_POISON, (size_t)(g_zh_cur - last_live_end));
        g_zh_cur = last_live_end; g_zh_slides++;
        if (zh_telem()) fprintf(stderr, "[ZH] slide(nomove) #%ld: used=%ld/%ld\n", g_zh_slides, (long)(g_zh_cur - g_zh_base), g_zh_cap);
        return;
    }
    char *src = g_zh_base, *dst = g_zh_base;
    while (src < g_zh_cur) {
        zh_hdr_t *hd = (zh_hdr_t *)src; long tot = (long)hd->total;
        if (hd->state == ZH_LIVE && hd->pin) { if (dst < src) g_zh_pin_holes++; dst = src + tot; src += tot; continue; }
        if (hd->state == ZH_LIVE) { if (dst < src) { memmove(dst, src, (size_t)tot); g_zh_slid_bytes += tot; g_zh_tab[((zh_hdr_t *)dst)->handle] = dst; } src += tot; dst += tot; continue; }
        if (hd->handle) { zh_handle_put(hd->handle); hd->handle = 0; }
        g_zh_deads++; src += tot;
    }
    if (dst < g_zh_cur) memset(dst, ZH_POISON, (size_t)(g_zh_cur - dst));
    g_zh_cur = dst; g_zh_slides++;
    if (zh_telem()) fprintf(stderr, "[ZH] slide #%ld: used=%ld/%ld\n", g_zh_slides, (long)(g_zh_cur - g_zh_base), g_zh_cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned rt_zh_alloc(long bytes, void **ubout)
{
    pthread_mutex_lock(&g_zh_mu);
    if (!g_zh_base) zh_init();
    long need = ZH_HDR + ((bytes + 15L) & ~15L);
    if (g_zh_cur + need > g_zh_base + g_zh_cap) zh_slide();
    if (g_zh_cur + need > g_zh_base + g_zh_cap) { fprintf(stderr, "[ZH] FATAL: heap exhausted after slide (need=%ld used=%ld cap=%ld) — raise SCRIP_ZH_MB\n", need, (long)(g_zh_cur - g_zh_base), g_zh_cap); abort(); }
    zh_hdr_t *hd = (zh_hdr_t *)g_zh_cur;
    hd->total = (uint32_t)need; hd->birth = ++g_zh_birth; hd->handle = zh_handle_get(); hd->state = ZH_LIVE; hd->pin = 1;
    g_zh_tab[hd->handle] = g_zh_cur;
    g_zh_cur += need; g_zh_allocs++;
    if (ubout) *ubout = (void *)((char *)hd + ZH_HDR);
    unsigned h = hd->handle;
    pthread_mutex_unlock(&g_zh_mu);
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zh_deref(unsigned h)
{
    pthread_mutex_lock(&g_zh_mu);
    char *b = (h && h < g_zh_tab_next) ? g_zh_tab[h] : 0;
    pthread_mutex_unlock(&g_zh_mu);
    if (!b) { fprintf(stderr, "[ZH] FATAL: deref of dead/invalid handle %u\n", h); abort(); }
    return (void *)(b + ZH_HDR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zh_pin(unsigned h)   { pthread_mutex_lock(&g_zh_mu); char *b = (h && h < g_zh_tab_next) ? g_zh_tab[h] : 0; if (!b) { fprintf(stderr, "[ZH] FATAL: pin of dead handle %u\n", h); abort(); } ((zh_hdr_t *)b)->pin++; pthread_mutex_unlock(&g_zh_mu); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zh_unpin(unsigned h) { pthread_mutex_lock(&g_zh_mu); char *b = (h && h < g_zh_tab_next) ? g_zh_tab[h] : 0; if (b && ((zh_hdr_t *)b)->pin) ((zh_hdr_t *)b)->pin--; pthread_mutex_unlock(&g_zh_mu); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zh_mark_dead(unsigned h)
{
    pthread_mutex_lock(&g_zh_mu);
    char *b = (h && h < g_zh_tab_next) ? g_zh_tab[h] : 0;
    if (b) { zh_hdr_t *hd = (zh_hdr_t *)b; hd->state = ZH_DEAD; zh_handle_put(hd->handle); hd->handle = 0; }
    pthread_mutex_unlock(&g_zh_mu);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned rt_zh_birthmark(void) { pthread_mutex_lock(&g_zh_mu); unsigned m = g_zh_birth + 1; pthread_mutex_unlock(&g_zh_mu); return m; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zh_kill_since(unsigned mark)
{
    pthread_mutex_lock(&g_zh_mu);
    char *src = g_zh_base;
    while (src && src < g_zh_cur) {
        zh_hdr_t *hd = (zh_hdr_t *)src;
        if (hd->state == ZH_LIVE && !hd->pin && hd->birth >= mark) { hd->state = ZH_DEAD; zh_handle_put(hd->handle); hd->handle = 0; }
        src += (long)hd->total;
    }
    pthread_mutex_unlock(&g_zh_mu);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_zh_live_count(void)
{
    long n = 0; char *src = g_zh_base;
    while (src && src < g_zh_cur) { zh_hdr_t *hd = (zh_hdr_t *)src; if (hd->state == ZH_LIVE) n++; src += (long)hd->total; }
    return n;
}
