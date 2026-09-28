#include "bb_pool.h"
#include "ct_vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
static uint8_t * pool_base  = NULL;
static uint8_t * pool_top   = NULL;
static uint8_t * pool_limit = NULL;
static long      page_size  = 0;
typedef struct { uint8_t * base; uint8_t * limit; size_t start; } bb_region_t;
static cv_t      pool_regions;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t * page_floor(uint8_t * p) { uintptr_t u = (uintptr_t)p;  return (uint8_t *)(u & ~(uintptr_t)(page_size - 1)); }
static uint8_t * page_ceil (uint8_t * p) { uintptr_t u = (uintptr_t)p; uintptr_t ps = (uintptr_t)page_size; return (uint8_t *)((u + ps - 1) & ~(ps - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_region_cur(void) {
    for (uint32_t k = 0; k < pool_regions.len; k++) if (CV_AT(pool_regions, bb_region_t, k).base == pool_base) return (int)k;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_region_new(size_t want, size_t start) {
    uint8_t * m = mmap(NULL, want, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (m == MAP_FAILED) { fprintf(stderr, "bb_pool: mmap of a %zu-byte code region failed\n", want); abort(); }
    CV_PUSH(pool_regions, bb_region_t) = (bb_region_t){ m, m + want, start };
    pool_base  = m;
    pool_top   = m;
    pool_limit = m + want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_init(void) {
    if (pool_base) return;
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) page_size = 4096;
    bb_region_new((size_t)BB_POOL_INIT, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_buf_t bb_alloc(size_t size) {
    uint8_t * start;
    uintptr_t ps;
    size_t pages, alloc;
    if (!pool_base) { fprintf(stderr, "bb_alloc: pool not initialised\n"); abort(); }
    start  = page_ceil(pool_top);
    ps     = (uintptr_t)page_size;
    pages  = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    alloc  = pages * (size_t)ps;
    while (start + alloc > pool_limit) {
        int k = bb_region_cur();
        bb_region_t r = CV_AT(pool_regions, bb_region_t, k);
        size_t cap = (size_t)(r.limit - r.base), want = 2 * cap;
        if ((uint32_t)k + 1 < pool_regions.len) {
            bb_region_t * n = &CV_AT(pool_regions, bb_region_t, k + 1); pool_base = n->base; pool_top = n->base; pool_limit = n->limit; start = pool_top; continue; }
        if (want < alloc) want = alloc;
        bb_region_new(want, r.start + cap);
        start = pool_top;
    }
    pool_top = start + alloc;
    return start;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_seal(bb_buf_t buf, size_t size) {
    uint8_t * lo  = page_floor(buf);
    uint8_t * hi  = page_ceil(buf + size);
    size_t    len = (size_t)(hi - lo);
    if (mprotect(lo, len, PROT_READ | PROT_EXEC) != 0) { perror("bb_seal: mprotect RW→RX"); abort(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_trim_last(bb_buf_t buf, size_t reserved, size_t used) {
    uintptr_t ps           = (uintptr_t)page_size;
    size_t    res_pages    = ((size_t)reserved + (size_t)ps - 1) / (size_t)ps;
    uint8_t * reserved_top = buf + res_pages * (size_t)ps;
    if (reserved_top != pool_top) return;
    uint8_t * want = page_ceil(buf + used);
    if (want < buf || want > pool_top) return;
    pool_top = want;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_free(bb_buf_t buf, size_t size) {
    uintptr_t ps    = (uintptr_t)page_size;
    size_t    pages = ((size_t)size + (size_t)ps - 1) / (size_t)ps;
    size_t    alloc = pages * (size_t)ps;
    if (buf + alloc != pool_top) {
        fprintf(stderr, "bb_free: LIFO violation — buf=%p + alloc=%zu != top=%p\n", (void *)buf, alloc, (void *)pool_top);
        abort();
    }
    pool_top = buf;
    if (mprotect(buf, alloc, PROT_READ | PROT_WRITE) != 0) { perror("bb_free: mprotect RX→RW"); abort(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t bb_pool_mark(void) { return pool_base ? CV_AT(pool_regions, bb_region_t, bb_region_cur()).start + (size_t)(pool_top - pool_base) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_release(size_t mark) {
    uint8_t * want;
    int cur, k;
    if (!pool_base) return;
    cur = bb_region_cur();
    for (k = cur; k > 0 && CV_AT(pool_regions, bb_region_t, k).start > mark; k--) ;
    { bb_region_t r = CV_AT(pool_regions, bb_region_t, k); want = r.base + (mark - r.start);
      if (mark < r.start || want > (k == cur ? pool_top : r.limit)) return;
      for (int j = k + 1; j <= cur; j++) { bb_region_t q = CV_AT(pool_regions, bb_region_t, j); uint8_t * qt = j == cur ? pool_top : q.limit;
          if (qt > q.base && mprotect(q.base, (size_t)(qt - q.base), PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_release: mprotect RX→RW"); abort(); } }
      if (k != cur) { pool_top = r.limit; pool_base = r.base; pool_limit = r.limit; } }
    if (want < pool_top) { size_t len = (size_t)(pool_top - want); if (mprotect(want, len, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_release: mprotect RX→RW"); abort(); } }
    pool_top = want;
}
