#ifndef G_RUNTIME_PAS_H
#define G_RUNTIME_PAS_H
#include <stddef.h>
#include "g_runtime_vec.h"
struct DESCR_t;
typedef struct { long pas_heap_ctr; struct DESCR_t *pas_heap; long pas_heap_cap; unsigned char *pas_heap_dead; long *pas_heap_free; long pas_heap_nfree; long pas_heap_freecap; gv_t pas_tf; } g_pas_t;
typedef struct { unsigned seed; unsigned old; unsigned i1; unsigned mt[624]; } g_pasrnd_t;
#endif
