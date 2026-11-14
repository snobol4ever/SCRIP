#ifndef RT_PL_TRAIL_PL_TR_ENTRY_H
#define RT_PL_TRAIL_PL_TR_ENTRY_H
#include <stdint.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { DESCR_t *cell; uint64_t pad; DESCR_t old; } pl_tr_entry_t;
#ifdef __cplusplus
}
#endif
#endif
