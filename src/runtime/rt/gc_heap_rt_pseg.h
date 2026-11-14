#ifndef GC_HEAP_RT_PSEG_H
#define GC_HEAP_RT_PSEG_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct rt_pseg_t {
    char *orig_lo;
    uint64_t len;
    char *tbase;
    uint64_t tn;
    uint64_t rbp, rbx, r12, r13, r14, r15;
    uint64_t planes[4];
    int64_t k_level;
    void *sched;
    uint64_t clen, sched_t, hdr;
    void *link;
} rt_pseg_t;
static inline char *rt_pseg_bytes(rt_pseg_t *b) { return (char *)(b + 1) + ((uintptr_t)b->orig_lo & 15u); }
static inline char *rt_pseg_trail(rt_pseg_t *b) { return (char *)(((uintptr_t)(rt_pseg_bytes(b) + b->len) + 15u) & ~(uintptr_t)15u); }
static inline size_t rt_pseg_size(const char *orig_lo, uint64_t len, uint64_t tn) { return sizeof(rt_pseg_t) + ((uintptr_t)orig_lo & 15u) + len + 16u + tn * 64u; }
#ifdef __cplusplus
}
#endif
#endif
