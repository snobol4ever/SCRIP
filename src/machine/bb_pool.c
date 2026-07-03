#include "bb_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
static uint8_t * pool_base  = NULL;
static uint8_t * pool_top   = NULL;
static uint8_t * pool_limit = NULL;
static long      page_size  = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t * page_floor(uint8_t * p) { uintptr_t u = (uintptr_t)p;  return (uint8_t *)(u & ~(uintptr_t)(page_size - 1)); }
static uint8_t * page_ceil (uint8_t * p) { uintptr_t u = (uintptr_t)p; uintptr_t ps = (uintptr_t)page_size; return (uint8_t *)((u + ps - 1) & ~(ps - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_init(void) {
    if (pool_base) return;
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) page_size = 4096;
    pool_base = mmap(NULL, BB_POOL_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (pool_base == MAP_FAILED) { perror("bb_pool_init: mmap"); abort(); }
    pool_top   = pool_base;
    pool_limit = pool_base + BB_POOL_SIZE;
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
    if (start + alloc > pool_limit) {
        return NULL;
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
size_t bb_pool_mark(void) { return pool_base ? (size_t)(pool_top - pool_base) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_pool_release(size_t mark) {
    uint8_t * want;
    if (!pool_base) return;
    want = pool_base + mark;
    if (want < pool_base || want > pool_top) return;
    if (want < pool_top) { size_t len = (size_t)(pool_top - want); if (mprotect(want, len, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_release: mprotect RX→RW"); abort(); } }
    pool_top = want;
}
