#ifndef BB_BOX_FN_H
#define BB_BOX_FN_H
#include <stddef.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef DESCR_t (*bb_box_fn)(void * zeta, int entry);
#ifdef __cplusplus
}
#endif
#endif
