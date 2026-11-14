#ifndef CORE_OPSYN2_H
#define CORE_OPSYN2_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#include "runtime/core/core_pattern_match_api.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline DESCR_t opsyn2(DESCR_t a, DESCR_t b) { return opsyn(a, b, NULVCL); }
#ifdef __cplusplus
}
#endif
#endif
