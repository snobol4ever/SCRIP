/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GC_AUDIT_B_H
#define GC_AUDIT_B_H
#ifdef SCRIP_GC_AUDIT_B
#include <stdint.h>
#include "gc_heap.h"
typedef struct gc_audit_b_skip_t { const void *at; long bytes; const char *name; } gc_audit_b_skip_t;
typedef struct gc_audit_b_rgn_t { const char *lo; const char *hi; const char *pop; const char *name; } gc_audit_b_rgn_t;
typedef struct gc_audit_b_t {
    const rt_hblk_t *(*blk_of)(const char *p);
    const rt_hblk_t *(*blk_at)(long i);
    long (*birth_of)(const char *blk, char *buf, long cap);
    const char *alo;
    const char *ahi;
    long run;
    long nblk;
    const gc_audit_b_rgn_t *rgn;
    long nrgn;
    const gc_audit_b_skip_t *skip;
    long nskip;
} gc_audit_b_t;
long gc_audit_b_collect(const gc_audit_b_t *v);
#endif
#endif
