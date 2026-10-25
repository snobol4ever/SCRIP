#ifndef G_RUNTIME_PLENG_H
#define G_RUNTIME_PLENG_H
#include <stdint.h>
typedef struct { void *inst; int64_t id; int64_t ctx; int64_t arena; int64_t slab; int pinned; } g_pl_eng_ent_t;
typedef struct { int64_t ctx; int64_t arena; int64_t slab; } g_pl_eng_pend_t;
typedef struct {
    g_pl_eng_ent_t *ent;
    int n;
    int cap;
    g_pl_eng_pend_t *pend;
    int npend;
    int cpend;
    int64_t *reaped;
    int nreaped;
    int creaped;
    int64_t nmade;
    int64_t nreaped_total;
    int64_t nfreed;
} g_pl_eng_t;
#endif
