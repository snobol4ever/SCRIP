#include "rt_slab.h"
#include "g_runtime.h"
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
static const size_t k_bytes[NKLASS] = { 64u << 10, 1u << 20, 16u << 20 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t pick_klass(size_t need) { for (uint32_t k = 0; k < NKLASS; k++) if (k_bytes[k] >= need) return k; return NKLASS; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_slab_t *rt_slab_try_get(size_t min_bytes) {
    uint32_t k = pick_klass(min_bytes);
    pthread_mutex_lock(&g_runtime.slab.mx);
    if (k < NKLASS && g_runtime.slab.free[k]) { rt_slab_t *s = g_runtime.slab.free[k]; g_runtime.slab.free[k] = s->next; pthread_mutex_unlock(&g_runtime.slab.mx); s->next = NULL; return s; }
    pthread_mutex_unlock(&g_runtime.slab.mx);
    size_t cap = (k < NKLASS) ? k_bytes[k] : ((min_bytes + 15u) & ~(size_t)15u);
    if (cap > SIZE_MAX - 32) return (rt_slab_t *)0;
    rt_slab_t *s = (rt_slab_t *)ct_try_alloc(32 + cap);
    if (!s) return (rt_slab_t *)0;
    s->next = NULL;
    s->cap = cap;
    s->klass = k;
    s->magic = RT_SLAB_MAGIC;
    pthread_mutex_lock(&g_runtime.slab.mx);
    g_runtime.slab.pool_bytes += 32 + cap;
    pthread_mutex_unlock(&g_runtime.slab.mx);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_slab_t *rt_slab_get(size_t min_bytes) { rt_slab_t *s = rt_slab_try_get(min_bytes); if (!s) { fprintf(stderr, "rt_slab_get: OOM (%zu)\n", min_bytes); abort(); } return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_slab_put(rt_slab_t *s) {
    if (!s) return;
    if (s->magic != RT_SLAB_MAGIC) { fprintf(stderr, "rt_slab_put: bad magic\n"); abort(); }
    if (s->klass >= NKLASS) { ct_drop(s); return; }
    pthread_mutex_lock(&g_runtime.slab.mx);
    s->next = g_runtime.slab.free[s->klass];
    g_runtime.slab.free[s->klass] = s;
    pthread_mutex_unlock(&g_runtime.slab.mx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t rt_slab_pool_bytes(void) { return g_runtime.slab.pool_bytes; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_slab_region(size_t n) { rt_slab_t *s = rt_slab_get(n); return RT_SLAB_BASE(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_slab_try_region(size_t n) { rt_slab_t *s = rt_slab_try_get(n); return s ? (void *)RT_SLAB_BASE(s) : (void *)0; }
