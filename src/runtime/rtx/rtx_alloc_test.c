#include <stdio.h>
#include <string.h>
#include <stdint.h>
typedef struct rt_hp_fr_t { char *top; char *end; long blocks; int armed; int _pad; char *virgin; int zfull; int _pad2; } rt_hp_fr_t;
extern rt_hp_fr_t g_hp_fr;
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
void *c_rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
char *rt_str_alloc(long n);
char *c_rt_str_alloc(long n);
void *rt_ws_alloc(size_t n);
void *c_rt_ws_alloc(size_t n);
typedef struct { long dtop, dvirgin, dblocks, poff; unsigned long size, type, flags; } delta_t;
static int fails = 0, n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static delta_t take(void *(*fn)(uint16_t, uint64_t), uint16_t ty, uint64_t pay) {
    char *t0 = g_hp_fr.top, *v0 = g_hp_fr.virgin; long b0 = g_hp_fr.blocks;
    char *p = (char *)fn(ty, pay);
    delta_t d;
    d.dtop = g_hp_fr.top - t0; d.dvirgin = g_hp_fr.virgin - v0; d.dblocks = g_hp_fr.blocks - b0; d.poff = p - t0;
    d.size = *(uint32_t *)(p - 8); d.type = *(uint16_t *)(p - 4); d.flags = *(uint16_t *)(p - 2);
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cmp(const char *what, delta_t a, delta_t c) {
    n++;
    if (memcmp(&a, &c, sizeof a) == 0) return;
    fails++;
    printf("  MISMATCH %-26s asm{top+%ld virgin+%ld blk+%ld poff=%ld sz=%lu ty=%lu fl=%lu}\n", what, a.dtop, a.dvirgin, a.dblocks, a.poff, a.size, a.type, a.flags);
    printf("  %-35s   c{top+%ld virgin+%ld blk+%ld poff=%ld sz=%lu ty=%lu fl=%lu}\n", "", c.dtop, c.dvirgin, c.dblocks, c.poff, c.size, c.type, c.flags);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pair(const char *what, uint16_t ty, uint64_t pay) {
    cmp(what, take(rt_gcheap_alloc, ty, pay), take(c_rt_gcheap_alloc, ty, pay));
    { delta_t c = take(c_rt_gcheap_alloc, ty, pay); cmp(what, take(rt_gcheap_alloc, ty, pay), c); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pair_str(const char *what, long len) {
    char *t0, *p; delta_t a, c;
    t0 = g_hp_fr.top; p = rt_str_alloc(len);
    a.dtop = g_hp_fr.top - t0; a.poff = p - t0; a.size = *(uint32_t *)(p - 8); a.type = *(uint16_t *)(p - 4); a.flags = *(uint16_t *)(p - 2); a.dvirgin = a.dblocks = 0;
    t0 = g_hp_fr.top; p = c_rt_str_alloc(len);
    c.dtop = g_hp_fr.top - t0; c.poff = p - t0; c.size = *(uint32_t *)(p - 8); c.type = *(uint16_t *)(p - 4); c.flags = *(uint16_t *)(p - 2); c.dvirgin = c.dblocks = 0;
    cmp(what, a, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_ws_alloc lives in a SEPARATE arena (g_wsi_*, hidden -- not in the .so's .dynsym, so this
 * external test binary cannot read it directly the way take()/pair() read the exported g_hp_fr).
 * Verified instead through the shared bump pointer's own fencepost: c_rt_ws_alloc and rt_ws_alloc
 * carve from the SAME g_wsi_ws frontier, so calling one then the other must land the second block
 * EXACTLY where the first one's title+size ends -- any drift in the asm's `total` computation
 * either overlaps the next title (corruption) or leaves a gap (both are wrong, both show here). */
static void pair_ws(const char *what, size_t sz) {
    char *pc = (char *)c_rt_ws_alloc(sz);
    uint32_t szc = *(uint32_t *)(pc - 8); uint16_t tyc = *(uint16_t *)(pc - 4); uint16_t flc = *(uint16_t *)(pc - 2);
    char *pa = (char *)rt_ws_alloc(sz);
    uint32_t sza = *(uint32_t *)(pa - 8); uint16_t tya = *(uint16_t *)(pa - 4); uint16_t fla = *(uint16_t *)(pa - 2);
    n++;
    /* next payload pointer = this payload pointer + this block's total size (16B title + payload,
     * rounded) -- pa must land exactly there, proving asm carved the SAME total off the SAME shared
     * frontier c_rt_ws_alloc just advanced. */
    if (tyc != 203 || flc != 1 || sza != szc || tya != tyc || fla != flc || pa != pc + szc) {
        fails++;
        printf("  MISMATCH ws %-18s c{sz=%u ty=%u fl=%u p=%p} asm{sz=%u ty=%u fl=%u p=%p} want_asm_p=%p\n",
               what, szc, tyc, flc, (void *)pc, sza, tya, fla, (void *)pa, (void *)(pc + szc));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    int i;
    for (i = 0; i < 8; i++) (void)c_rt_gcheap_alloc(1, 64);
    if (!g_hp_fr.armed) { printf("RTX alloc unit: heap never armed — cannot compare fast paths\n"); return 2; }
    pair("payload 0",        1, 0);
    pair("payload 1",        1, 1);
    pair("payload 15",       1, 15);
    pair("payload 16",       1, 16);
    pair("payload 17",       1, 17);
    pair("payload 31",       1, 31);
    pair("payload 32",       1, 32);
    pair("payload 33",       1, 33);
    pair("payload 4096",     1, 4096);
    pair("type HB_WSC-ish",  201, 48);
    pair("type HB_PLJ 209",  209, 32);
    pair("type DT_S big",    1, 65536);
    pair("type 0",           0, 24);
    pair("type 65535",       65535, 24);
    pair_str("str_alloc 0",   0);
    pair_str("str_alloc 1",   1);
    pair_str("str_alloc 15",  15);
    pair_str("str_alloc 16",  16);
    pair_str("str_alloc 31",  31);
    pair_str("str_alloc -1",  -1);
    pair_str("str_alloc -99", -99);
    pair_str("str_alloc 1000", 1000);
    (void)c_rt_ws_alloc(1);   /* prime g_wsi_base so rt_ws_alloc's own first call below hits the fast path, not the lazy-init bail */
    pair_ws("ws payload 0",    0);
    pair_ws("ws payload 1",    1);
    pair_ws("ws payload 15",   15);
    pair_ws("ws payload 16",   16);
    pair_ws("ws payload 17",   17);
    pair_ws("ws payload 31",   31);
    pair_ws("ws payload 32",   32);
    pair_ws("ws payload 33",   33);
    pair_ws("ws payload 256",  256);
    pair_ws("ws payload 4096", 4096);
    printf("RTX alloc unit: %d checks, %d mismatches -> %s\n", n, fails, fails ? "FAIL" : "PASS");
    return fails != 0;
}
