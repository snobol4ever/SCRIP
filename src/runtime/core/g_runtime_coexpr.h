#ifndef G_RUNTIME_COEXPR_H
#define G_RUNTIME_COEXPR_H
#include <pthread.h>
typedef struct { int inited; pthread_attr_t attribs; long coexp_stksize; pthread_t co_main_thr; int co_main_set; long coexpr_serial; char root_ctx[1024] __attribute__((aligned(16))); } g_coexpr_t;
#ifdef __cplusplus
extern "C" {
#endif
extern g_coexpr_t g_rt_coexpr;
#ifdef __cplusplus
}
#endif
#endif
