#include "bb_pool.h"
#include "lower/g_lower_ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t * page_floor(uint8_t * p) { uintptr_t u = (uintptr_t)p; return (uint8_t *)(u & ~(uintptr_t)(g_lower_ir.bb_pool_page_size - 1)); }
static uint8_t * page_ceil (uint8_t * p) { uintptr_t u = (uintptr_t)p; uintptr_t ps = (uintptr_t)g_lower_ir.bb_pool_page_size; return (uint8_t *)((u + ps - 1) & ~(ps - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_init(void) {
    if (g_lower_ir.bb_pool_base) return;
    g_lower_ir.bb_pool_page_size = sysconf(_SC_PAGESIZE);
    if (g_lower_ir.bb_pool_page_size <= 0) g_lower_ir.bb_pool_page_size = 4096;
    g_lower_ir.bb_pool_base = mmap(NULL, BB_POOL_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (g_lower_ir.bb_pool_base == MAP_FAILED) { perror("bb_pool_init: mmap"); abort(); }
    g_lower_ir.bb_pool_top = g_lower_ir.bb_pool_base;
    {
        const char * e = getenv("SCRIP_CODE_POOL_MB");
        unsigned long mb = (e && *e) ? strtoul(e, NULL, 10) : 0;
        size_t sz = BB_POOL_SIZE;
        if (mb >= 8 && (mb << 20) < sz) sz = (size_t)mb << 20;
        g_lower_ir.bb_pool_limit = g_lower_ir.bb_pool_base + sz;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_buf_t bb_alloc(size_t size) {
    uint8_t * start;
    uintptr_t ps;
    size_t pages, alloc;
    if (!g_lower_ir.bb_pool_base) { fprintf(stderr, "bb_alloc: pool not initialised\n"); abort(); }
    start = page_ceil(g_lower_ir.bb_pool_top);
    ps = (uintptr_t)g_lower_ir.bb_pool_page_size;
    pages = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    alloc = pages * (size_t)ps;
    if (start + alloc > g_lower_ir.bb_pool_limit) { return NULL; }
    g_lower_ir.bb_pool_top = start + alloc;
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
    uintptr_t ps = (uintptr_t)g_lower_ir.bb_pool_page_size;
    size_t res_pages = ((size_t)reserved + (size_t)ps - 1) / (size_t)ps;
    uint8_t * reserved_top = buf + res_pages * (size_t)ps;
    if (reserved_top != g_lower_ir.bb_pool_top) return;
    uint8_t * want = page_ceil(buf + used);
    if (want < buf || want > g_lower_ir.bb_pool_top) return;
    g_lower_ir.bb_pool_top = want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_free(bb_buf_t buf, size_t size) {
    uintptr_t ps = (uintptr_t)g_lower_ir.bb_pool_page_size;
    size_t pages = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    size_t alloc = pages * (size_t)ps;
    if (buf + alloc != g_lower_ir.bb_pool_top) { fprintf(stderr, "bb_free: LIFO violation — buf=%p + alloc=%zu != top=%p\n", (void *)buf, alloc, (void *)g_lower_ir.bb_pool_top); abort(); }
    g_lower_ir.bb_pool_top = buf;
    if (mprotect(buf, alloc, PROT_READ | PROT_WRITE) != 0) { perror("bb_free: mprotect RX→RW"); abort(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t bb_pool_mark(void) { return g_lower_ir.bb_pool_base ? (size_t)(g_lower_ir.bb_pool_top - g_lower_ir.bb_pool_base) : 0; }
const uint8_t * bb_pool_base(void) { return g_lower_ir.bb_pool_base; }
long bb_pool_page(void) { return g_lower_ir.bb_pool_page_size; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void g_emit_stno_drop_above(uint64_t addr);
extern void g_emit_stno_drop_range(uint64_t lo, uint64_t hi);
extern void rt_gc_frame_maps_drop_range(const void * lo, const void * hi);
extern void rt_gc_frame_sites_drop_range(const void * lo, const void * hi);
typedef struct { size_t lo, hi; } bb_hole_t;
#define BB_HOLE(i) CV_AT(g_lower_ir.bb_holes, bb_hole_t, (i))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t hole_find(size_t lo) { uint32_t a = 0, b = g_lower_ir.bb_holes.len; while (a < b) { uint32_t m = (a + b) / 2; if (BB_HOLE(m).lo < lo) a = m + 1; else b = m; } return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void hole_erase(uint32_t i, uint32_t n) { memmove(&BB_HOLE(i), &BB_HOLE(i + n), (size_t)(g_lower_ir.bb_holes.len - i - n) * sizeof(bb_hole_t)); g_lower_ir.bb_holes.len -= n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void hole_insert(uint32_t i, size_t lo, size_t hi) {
    cv_reserve(&g_lower_ir.bb_holes, (uint32_t)sizeof(bb_hole_t), (uint64_t)g_lower_ir.bb_holes.len + 1, "bb_holes");
    memmove(&BB_HOLE(i + 1), &BB_HOLE(i), (size_t)(g_lower_ir.bb_holes.len - i) * sizeof(bb_hole_t));
    BB_HOLE(i).lo = lo;
    BB_HOLE(i).hi = hi;
    g_lower_ir.bb_holes.len++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void hole_truncate(size_t top) {
    uint32_t i = hole_find(top);
    if (i < g_lower_ir.bb_holes.len) hole_erase(i, g_lower_ir.bb_holes.len - i);
    if (i > 0 && BB_HOLE(i - 1).hi > top) { if (BB_HOLE(i - 1).lo >= top) hole_erase(i - 1, 1); else BB_HOLE(i - 1).hi = top; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_release(size_t mark) {
    uint8_t * want;
    if (!g_lower_ir.bb_pool_base) return;
    want = g_lower_ir.bb_pool_base + mark;
    if (want < g_lower_ir.bb_pool_base || want > g_lower_ir.bb_pool_top) return;
    if (want < g_lower_ir.bb_pool_top) {
        size_t len = (size_t)(g_lower_ir.bb_pool_top - want);
        if (mprotect(want, len, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_release: mprotect RX→RW"); abort(); }
    }
    g_emit_stno_drop_above((uint64_t)(uintptr_t)want);
    rt_gc_frame_maps_drop_range(want, g_lower_ir.bb_pool_top);
    rt_gc_frame_sites_drop_range(want, g_lower_ir.bb_pool_top);
    hole_truncate(mark);
    g_lower_ir.bb_pool_top = want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_reclaim(const void * lo_p, const void * hi_p) {
    uint8_t * base = g_lower_ir.bb_pool_base;
    size_t lo, hi, top;
    uint32_t i;
    if (!base || (const uint8_t *)lo_p < base) return;
    lo = (size_t)((const uint8_t *)lo_p - base);
    hi = (size_t)((const uint8_t *)hi_p - base);
    top = (size_t)(g_lower_ir.bb_pool_top - base);
    if (hi > top) hi = top;
    if (hi <= lo) return;
    i = hole_find(lo);
    if ((i > 0 && BB_HOLE(i - 1).hi > lo) || (i < g_lower_ir.bb_holes.len && BB_HOLE(i).lo < hi)) {
        fprintf(stderr, "bb_pool_reclaim: range [%zx,%zx) overlaps a hole already on the list\n", lo, hi);
        abort();
    }
    if (i > 0 && BB_HOLE(i - 1).hi == lo) { lo = BB_HOLE(i - 1).lo; i--; hole_erase(i, 1); }
    if (i < g_lower_ir.bb_holes.len && BB_HOLE(i).lo == hi) { hi = BB_HOLE(i).hi; hole_erase(i, 1); }
    if (hi == top) { if (mprotect(base + lo, hi - lo, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_reclaim: mprotect NONE→RW"); abort(); } g_lower_ir.bb_pool_top = base + lo; return; }
    hole_insert(i, lo, hi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_buf_t bb_alloc_hole(size_t min_size, size_t * cap) {
    uint8_t * base = g_lower_ir.bb_pool_base;
    for (uint32_t i = 0; base && i < g_lower_ir.bb_holes.len; i++) {
        size_t n = BB_HOLE(i).hi - BB_HOLE(i).lo;
        uint8_t * p = base + BB_HOLE(i).lo;
        if (n < min_size) continue;
        if (mprotect(p, n, PROT_READ | PROT_WRITE) != 0) { perror("bb_alloc_hole: mprotect NONE→RW"); abort(); }
        hole_erase(i, 1);
        *cap = n;
        return p;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void hole_give_back(uint8_t * lo, uint8_t * hi) {
    if (hi <= lo) return;
    if (mprotect(lo, (size_t)(hi - lo), PROT_NONE) != 0) { perror("bb_hole_return: mprotect RW→NONE"); abort(); }
    madvise(lo, (size_t)(hi - lo), MADV_DONTNEED);
    bb_pool_reclaim(lo, hi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_hole_return(bb_buf_t buf, size_t keep, size_t cap) { hole_give_back(page_ceil(buf + keep), buf + cap); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_hole_abort(bb_buf_t buf, size_t cap) {
    g_emit_stno_drop_range((uint64_t)(uintptr_t)buf, (uint64_t)(uintptr_t)(buf + cap));
    rt_gc_frame_maps_drop_range(buf, buf + cap);
    rt_gc_frame_sites_drop_range(buf, buf + cap);
    hole_give_back(buf, buf + cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_pool_in_hole(const void * p) {
    uint32_t i;
    size_t o;
    if (!g_lower_ir.bb_pool_base || (const uint8_t *)p < g_lower_ir.bb_pool_base) return 0;
    o = (size_t)((const uint8_t *)p - g_lower_ir.bb_pool_base);
    i = hole_find(o + 1);
    return i > 0 && BB_HOLE(i - 1).lo <= o && o < BB_HOLE(i - 1).hi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_last_set(bb_buf_t buf, size_t used) { g_lower_ir.bb_last_lo = buf; g_lower_ir.bb_last_hi = page_ceil(buf + used); }
const uint8_t * bb_pool_last_lo(void) { return g_lower_ir.bb_last_lo; }
const uint8_t * bb_pool_last_hi(void) { return g_lower_ir.bb_last_hi; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t bb_pool_used(void) { return g_lower_ir.bb_pool_base ? (size_t)(g_lower_ir.bb_pool_top - g_lower_ir.bb_pool_base) : 0; }
size_t bb_pool_free(void) { return g_lower_ir.bb_pool_base ? (size_t)(g_lower_ir.bb_pool_limit - page_ceil(g_lower_ir.bb_pool_top)) : 0; }
