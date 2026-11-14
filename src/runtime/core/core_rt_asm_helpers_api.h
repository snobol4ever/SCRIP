#ifndef CORE_RT_ASM_HELPERS_API_H
#define CORE_RT_ASM_HELPERS_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
int64_t to_int(DESCR_t v);
DESCR_t rt_deref(DESCR_t d);
#ifdef __cplusplus
}
#endif
#endif
