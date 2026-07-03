#ifndef PL_AREA_H
#define PL_AREA_H
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#if defined(__linux__)
extern void *mremap(void *, size_t, size_t, int, ...);
#endif
typedef struct { char *base; char *top; char *limit; size_t cap; } pl_area_t;
#ifndef PL_AREA_DEFAULT_BYTES
#define PL_AREA_DEFAULT_BYTES ((size_t)256u * 1024u * 1024u)
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_area_init(pl_area_t *a, size_t bytes) {
    if (bytes < 4096u) bytes = 4096u;
    void *p = mmap((void *)0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) { a->base = a->top = a->limit = (char *)0; a->cap = 0; return; }
    a->base = (char *)p; a->top = (char *)p; a->limit = (char *)p + bytes; a->cap = bytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_area_grow(pl_area_t *a, size_t need) {
    if (!a->base) return 0;
    size_t used = (size_t)(a->top - a->base);
    size_t nc = a->cap ? a->cap : 4096u;
    while (nc - used < need) nc *= 2;
#if defined(__linux__)
    void *p = mremap(a->base, a->cap, nc, 0);
    if (p == MAP_FAILED) return 0;
    a->base = (char *)p; a->top = (char *)p + used; a->limit = (char *)p + nc; a->cap = nc; return 1;
#else
    (void)nc; return 0;
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void *pl_area_bump(pl_area_t *a, size_t bytes) {
    bytes = (bytes + 15u) & ~((size_t)15u);
    if (a->top + bytes > a->limit) { if (!pl_area_grow(a, bytes)) return (void *)0; }
    void *r = a->top; a->top += bytes; return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char *pl_area_mark(const pl_area_t *a) { return a->top; }
static inline void  pl_area_reset(pl_area_t *a, char *mark) { a->top = mark; }
static inline size_t pl_area_used(const pl_area_t *a) { return (size_t)(a->top - a->base); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void *pl_env_bump(pl_area_t *a, int nbytes) {
    if (!a->base) pl_area_init(a, PL_AREA_DEFAULT_BYTES);
    return pl_area_bump(a, (size_t)(nbytes > 0 ? nbytes : 16));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int  pl_env_mark(const pl_area_t *a) { return a->base ? (int)(a->top - a->base) : 0; }
static inline void pl_env_reset(pl_area_t *a, int off) { if (a->base && off >= 0) a->top = a->base + off; }
#endif
