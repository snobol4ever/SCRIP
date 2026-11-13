#ifndef G_RUNTIME_H
#define G_RUNTIME_H
#include <stddef.h>
#include "g_runtime_agg.h"
#include "g_runtime_gc.h"
#include "g_runtime_abi.h"
typedef struct { g_abi_t abi; g_agg_t agg; g_gc_t gc; int scrip_env; } g_runtime_t;
#define GRT_AT(m) ((long)offsetof(g_runtime_t, abi.m))
#ifdef __cplusplus
extern "C" {
#endif
extern g_runtime_t g_runtime;
const char *rt_scrip_getenv(const char *name);
int rt_scrip_env_bits(void);
#ifdef __cplusplus
}
#endif
#endif
