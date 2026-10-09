#include "bb_pool.h"
#include "g_lower.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t * page_floor(uint8_t * p) { uintptr_t u = (uintptr_t)p; return (uint8_t *)(u & ~(uintptr_t)(g_lower.ir.bb_pool_page_size - 1)); }
static uint8_t * page_ceil (uint8_t * p) { uintptr_t u = (uintptr_t)p; uintptr_t ps = (uintptr_t)g_lower.ir.bb_pool_page_size; return (uint8_t *)((u + ps - 1) & ~(ps - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_init(void) {
    if (g_lower.ir.bb_pool_base) return;
    g_lower.ir.bb_pool_page_size = sysconf(_SC_PAGESIZE);
    if (g_lower.ir.bb_pool_page_size <= 0) g_lower.ir.bb_pool_page_size = 4096;
    g_lower.ir.bb_pool_base = mmap(NULL, BB_POOL_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (g_lower.ir.bb_pool_base == MAP_FAILED) { perror("bb_pool_init: mmap"); abort(); }
    g_lower.ir.bb_pool_top = g_lower.ir.bb_pool_base;
    {
        const char * e = getenv("SCRIP_CODE_POOL_MB");
        unsigned long mb = (e && *e) ? strtoul(e, NULL, 10) : 0;
        size_t sz = BB_POOL_SIZE;
        if (mb >= 8 && (mb << 20) < sz) sz = (size_t)mb << 20;
        g_lower.ir.bb_pool_limit = g_lower.ir.bb_pool_base + sz;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_buf_t bb_alloc(size_t size) {
    uint8_t * start;
    uintptr_t ps;
    size_t pages, alloc;
    if (!g_lower.ir.bb_pool_base) { fprintf(stderr, "bb_alloc: pool not initialised\n"); abort(); }
    start = page_ceil(g_lower.ir.bb_pool_top);
    ps = (uintptr_t)g_lower.ir.bb_pool_page_size;
    pages = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    alloc = pages * (size_t)ps;
    if (start + alloc > g_lower.ir.bb_pool_limit) { return NULL; }
    g_lower.ir.bb_pool_top = start + alloc;
    return start;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_seal(bb_buf_t buf, size_t size) {
    uint8_t * lo = page_floor(buf);
    uint8_t * hi = page_ceil(buf + size);
    size_t len = (size_t)(hi - lo);
    if (mprotect(lo, len, PROT_READ | PROT_EXEC) != 0) { perror("bb_seal: mprotect RW→RX"); abort(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_trim_last(bb_buf_t buf, size_t reserved, size_t used) {
    uintptr_t ps = (uintptr_t)g_lower.ir.bb_pool_page_size;
    size_t res_pages = ((size_t)reserved + (size_t)ps - 1) / (size_t)ps;
    uint8_t * reserved_top = buf + res_pages * (size_t)ps;
    if (reserved_top != g_lower.ir.bb_pool_top) return;
    uint8_t * want = page_ceil(buf + used);
    if (want < buf || want > g_lower.ir.bb_pool_top) return;
    g_lower.ir.bb_pool_top = want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_free(bb_buf_t buf, size_t size) {
    uintptr_t ps = (uintptr_t)g_lower.ir.bb_pool_page_size;
    size_t pages = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    size_t alloc = pages * (size_t)ps;
    if (buf + alloc != g_lower.ir.bb_pool_top) { fprintf(stderr, "bb_free: LIFO violation — buf=%p + alloc=%zu != top=%p\n", (void *)buf, alloc, (void *)g_lower.ir.bb_pool_top); abort(); }
    g_lower.ir.bb_pool_top = buf;
    if (mprotect(buf, alloc, PROT_READ | PROT_WRITE) != 0) { perror("bb_free: mprotect RX→RW"); abort(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t bb_pool_mark(void) { return g_lower.ir.bb_pool_base ? (size_t)(g_lower.ir.bb_pool_top - g_lower.ir.bb_pool_base) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void g_emit_stno_drop_above(uint64_t addr);
extern void rt_gc_frame_maps_drop_range(const void * lo, const void * hi);
extern void rt_gc_frame_sites_drop_range(const void * lo, const void * hi);
void bb_pool_release(size_t mark) {
    uint8_t * want;
    if (!g_lower.ir.bb_pool_base) return;
    want = g_lower.ir.bb_pool_base + mark;
    if (want < g_lower.ir.bb_pool_base || want > g_lower.ir.bb_pool_top) return;
    if (want < g_lower.ir.bb_pool_top) {
        size_t len = (size_t)(g_lower.ir.bb_pool_top - want);
        if (mprotect(want, len, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_release: mprotect RX→RW"); abort(); }
    }
    g_emit_stno_drop_above((uint64_t)(uintptr_t)want);
    rt_gc_frame_maps_drop_range(want, g_lower.ir.bb_pool_top);
    rt_gc_frame_sites_drop_range(want, g_lower.ir.bb_pool_top);
    g_lower.ir.bb_pool_top = want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t bb_pool_used(void) { return g_lower.ir.bb_pool_base ? (size_t)(g_lower.ir.bb_pool_top - g_lower.ir.bb_pool_base) : 0; }
size_t bb_pool_free(void) { return g_lower.ir.bb_pool_base ? (size_t)(g_lower.ir.bb_pool_limit - page_ceil(g_lower.ir.bb_pool_top)) : 0; }
