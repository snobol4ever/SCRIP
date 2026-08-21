#include "rt_slab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#define NKLASS 3
static const size_t k_bytes[NKLASS] = { 64u << 10, 1u << 20, 16u << 20 };
static rt_slab_t      *g_free[NKLASS];
static size_t          g_pool_bytes = 0;
static pthread_mutex_t g_mx = PTHREAD_MUTEX_INITIALIZER;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t pick_klass(size_t need) {
    for (uint32_t k = 0; k < NKLASS; k++) if (k_bytes[k] >= need) return k;
    return NKLASS;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_slab_t *rt_slab_get(size_t min_bytes) {
    uint32_t k = pick_klass(min_bytes);
    pthread_mutex_lock(&g_mx);
    if (k < NKLASS && g_free[k]) {
        rt_slab_t *s = g_free[k];
        g_free[k] = s->next;
        pthread_mutex_unlock(&g_mx);
        s->next = NULL;
        return s;
    }
    pthread_mutex_unlock(&g_mx);
    size_t cap = (k < NKLASS) ? k_bytes[k] : ((min_bytes + 15u) & ~(size_t)15u);
    rt_slab_t *s = (rt_slab_t *)malloc(32 + cap);
    if (!s) { fprintf(stderr, "rt_slab_get: OOM (%zu)\n", cap); abort(); }
    s->next = NULL; s->cap = cap; s->klass = k; s->magic = RT_SLAB_MAGIC;
    pthread_mutex_lock(&g_mx);
    g_pool_bytes += 32 + cap;
    pthread_mutex_unlock(&g_mx);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_slab_put(rt_slab_t *s) {
    if (!s) return;
    if (s->magic != RT_SLAB_MAGIC) { fprintf(stderr, "rt_slab_put: bad magic\n"); abort(); }
    if (s->klass >= NKLASS) { free(s); return; }
    pthread_mutex_lock(&g_mx);
    s->next = g_free[s->klass];
    g_free[s->klass] = s;
    pthread_mutex_unlock(&g_mx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t rt_slab_pool_bytes(void) { return g_pool_bytes; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_slab_region(size_t n) {
    rt_slab_t *s = rt_slab_get(n);
    return RT_SLAB_BASE(s);
}
