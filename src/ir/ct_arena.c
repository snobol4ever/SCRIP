/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "ct_arena.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#define CT_ALIGN     ((size_t)32)
#define CT_BINS      20
#define CT_MIN_CLASS ((size_t)16)
#define CT_CHUNK     ((size_t)1 << 22)
#define CT_MAGIC_BIN ((uint64_t)0x4354424c4f434b31ULL)
#define CT_MAGIC_BIG ((uint64_t)0x4354424c4f434b32ULL)
typedef struct ct_head { uint64_t magic; uint64_t size; struct ct_head *next; uint64_t pad; } ct_head_t;
static ct_head_t *ct_bin[CT_BINS];
static int        ct_poison = -1;
static uint8_t *ct_cur = (uint8_t *)0;
static uint8_t *ct_end = (uint8_t *)0;
static size_t   ct_taken = 0;
static size_t   ct_mapped = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t ct_page(void) { long p = sysconf(_SC_PAGESIZE); return p > 0 ? (size_t)p : (size_t)4096; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *ct_map(size_t want) {
    void *m = mmap((void *)0, want, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (m == MAP_FAILED) { fprintf(stderr, "ct_arena: mmap of %zu bytes failed\n", want); _exit(3); }
    ct_mapped += want;
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ct_class_of(size_t n) {
    size_t cap = CT_MIN_CLASS;
    int i = 0;
    while (i < CT_BINS) { if (n <= cap) return i; cap <<= 1; i++; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t ct_cap_of(int cls) { return CT_MIN_CLASS << (size_t)cls; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *ct_alloc(size_t n) {
    int cls;
    size_t cap, need, pg;
    ct_head_t *h;
    if (n == 0) n = 1;
    cls = ct_class_of(n);
    if (cls < 0) {
        pg = ct_page();
        need = ((n + sizeof(ct_head_t)) + pg - 1) & ~(pg - 1);
        h = (ct_head_t *)ct_map(need);
        h->magic = CT_MAGIC_BIG;
        h->size  = need;
        ct_taken += need;
        return (void *)((uint8_t *)h + CT_ALIGN);
    }
    if (ct_bin[cls]) { h = ct_bin[cls]; ct_bin[cls] = h->next; h->magic = CT_MAGIC_BIN; h->size = ct_cap_of(cls); h->next = (ct_head_t *)0; ct_taken += h->size; return (void *)((uint8_t *)h + CT_ALIGN); }
    cap  = ct_cap_of(cls);
    need = cap + CT_ALIGN;
    if ((size_t)(ct_end - ct_cur) < need) { size_t want = CT_CHUNK > need ? CT_CHUNK : ((need + ct_page() - 1) & ~(ct_page() - 1)); ct_cur = (uint8_t *)ct_map(want); ct_end = ct_cur + want; }
    h = (ct_head_t *)ct_cur;
    ct_cur += need;
    h->magic = CT_MAGIC_BIN;
    h->size  = cap;
    ct_taken += cap;
    return (void *)((uint8_t *)h + CT_ALIGN);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *ct_zalloc(size_t n, size_t sz) {
    size_t want = n * sz;
    void *p = ct_alloc(want ? want : 1);
    memset(p, 0, want ? want : 1);
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *ct_grow(void *p, size_t n) {
    ct_head_t *h;
    void *q;
    size_t old, cap;
    int cls;
    if (!p) return ct_alloc(n);
    h = (ct_head_t *)((uint8_t *)p - CT_ALIGN);
    if (h->magic != CT_MAGIC_BIN && h->magic != CT_MAGIC_BIG) { fprintf(stderr, "ct_arena: ct_grow on a block this allocator did not hand out (%p)\n", p); _exit(3); }
    old = h->magic == CT_MAGIC_BIN ? (size_t)h->size : (size_t)h->size - CT_ALIGN;
    if (n <= old) return p;
    cls = ct_class_of(n);
    if (h->magic == CT_MAGIC_BIN && cls >= 0 && (uint8_t *)p + old == ct_cur && (size_t)(ct_end - (uint8_t *)p) >= (cap = ct_cap_of(cls))) {
        ct_cur = (uint8_t *)p + cap; ct_taken += cap - old; h->size = cap; return p;
    }
    q = ct_alloc(n);
    memcpy(q, p, old);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ct_drop(void *p) {
    ct_head_t *h;
    int cls;
    if (!p) return;
    h = (ct_head_t *)((uint8_t *)p - CT_ALIGN);
    if (h->magic == CT_MAGIC_BIN) {
        cls = ct_class_of((size_t)h->size);
        if (cls < 0) return;
        if (ct_poison < 0) ct_poison = getenv("SCRIP_CT_POISON") ? 1 : 0;
        if (ct_poison) memset(p, 0xDD, (size_t)h->size);
        if (getenv("SCRIP_CT_NORECYCLE")) { if (ct_poison) h->magic = 0; return; }
        h->magic = 0;
        ct_taken -= (size_t)h->size;
        h->next = ct_bin[cls];
        ct_bin[cls] = h;
        return;
    }
    if (h->magic == CT_MAGIC_BIG) { size_t len = (size_t)h->size; h->magic = 0; ct_taken -= len; munmap((void *)h, len); return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *ct_strdup(const char *s) {
    size_t n;
    char *q;
    if (!s) return (char *)0;
    n = strlen(s);
    q = (char *)ct_alloc(n + 1);
    memcpy(q, s, n + 1);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *ct_strndup(const char *s, size_t n) {
    size_t k = 0;
    char *q;
    if (!s) return (char *)0;
    while (k < n && s[k]) k++;
    q = (char *)ct_alloc(k + 1);
    memcpy(q, s, k);
    q[k] = '\0';
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t ct_arena_bytes(void) { return ct_taken; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t ct_arena_mapped(void) { return ct_mapped; }
