/* ATTIC: dead code excised from src/machine/bb_pool.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_in_pool(const void * p) { return pool_base && (const uint8_t *)p >= pool_base && (const uint8_t *)p < pool_limit; }
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_pool_reset(void) {
    uint8_t * lo;
    uint8_t * hi;
    size_t    len;
    if (!pool_base) return;
    if (pool_top > pool_base) {
        lo  = pool_base;
        hi  = page_ceil(pool_top);
        len = (size_t)(hi - lo);
        if (mprotect(lo, len, PROT_READ | PROT_WRITE) != 0) { perror("bb_pool_reset: mprotect RX→RW"); abort(); }
    }
    pool_top = pool_base;
}
