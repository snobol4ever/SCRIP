#ifndef G_RUNTIME_COEXPR_H
#define G_RUNTIME_COEXPR_H
#include <pthread.h>
typedef struct { int inited; pthread_attr_t attribs; long coexp_stksize; pthread_t co_main_thr; int co_main_set; long coexpr_serial; } g_coexpr_t;
#endif
