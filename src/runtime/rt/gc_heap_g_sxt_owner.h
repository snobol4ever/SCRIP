#ifndef GC_HEAP_G_SXT_OWNER_H
#define GC_HEAP_G_SXT_OWNER_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/core/g_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
#define g_sxt_owner (g_runtime.abi.sxt_fr.owner)
#ifdef __cplusplus
}
#endif
#endif
