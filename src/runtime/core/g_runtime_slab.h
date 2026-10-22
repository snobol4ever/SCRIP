#ifndef G_RUNTIME_SLAB_H
#define G_RUNTIME_SLAB_H
#include <pthread.h>
#include <stddef.h>
#define NKLASS 3
typedef struct { struct rt_slab *free[NKLASS]; size_t pool_bytes; pthread_mutex_t mx; } g_slab_t;
#endif
