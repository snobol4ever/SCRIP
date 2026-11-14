#ifndef CORE_RUNTIME_EVAL_API_H
#define CORE_RUNTIME_EVAL_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t EXPVAL_fn(DESCR_t expr_d);
int rt_eval_open_takes(DESCR_t a);
DESCR_t CONVE_fn(DESCR_t str_d);
#ifdef __cplusplus
}
#endif
#endif
