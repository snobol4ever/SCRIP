#ifndef RT_PL_TRAIL_PL_TR_CTX_S_H
#define RT_PL_TRAIL_PL_TR_CTX_S_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct pl_tr_ctx_s { char *tr; char *b; void *ball; uint64_t rbx; uint64_t r14; uint64_t r15; } pl_tr_ctx_t;
#ifdef __cplusplus
}
#endif
#endif
