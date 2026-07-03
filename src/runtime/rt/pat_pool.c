#include "dtp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#define PAT_POOL_SIZE (4u << 20)
uint8_t *g_pat_pool_base = NULL;
uint8_t *g_pat_pool_cur  = NULL;
uint8_t *g_pat_pool_end  = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pat_pool_init(void) {
    if (g_pat_pool_base) return;
    g_pat_pool_base = mmap(NULL, PAT_POOL_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (g_pat_pool_base == MAP_FAILED) { perror("pat_pool_init: mmap RWX"); abort(); }
    g_pat_pool_cur = g_pat_pool_base;
    g_pat_pool_end = g_pat_pool_base + PAT_POOL_SIZE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pat_pool_reset(void) {
    g_pat_pool_cur = g_pat_pool_base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pat_pool_emit(const uint8_t *code, uint32_t n) {
    if (!g_pat_pool_base) pat_pool_init();
    if (g_pat_pool_cur + n + 16 > g_pat_pool_end) { fprintf(stderr, "pat_pool_emit: pool exhausted (%u)\n", n); abort(); }
    uint8_t *entry = g_pat_pool_cur;
    memcpy(entry, code, (size_t)n);
    g_pat_pool_cur += (n + 15u) & ~15u;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void pat_pool_ctor(void) { pat_pool_init(); }
