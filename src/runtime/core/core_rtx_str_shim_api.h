#ifndef CORE_RTX_STR_SHIM_API_H
#define CORE_RTX_STR_SHIM_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
char *VARVAL_fn(DESCR_t v);
#ifdef __cplusplus
}
#endif
#endif
