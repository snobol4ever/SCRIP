#ifndef CORE_ARITHMETIC_API_H
#define CORE_ARITHMETIC_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
int lt(DESCR_t a, DESCR_t b);
int le(DESCR_t a, DESCR_t b);
int gt(DESCR_t a, DESCR_t b);
int ge(DESCR_t a, DESCR_t b);
DESCR_t add(DESCR_t a, DESCR_t b);
DESCR_t sub(DESCR_t a, DESCR_t b);
DESCR_t mul(DESCR_t a, DESCR_t b);
DESCR_t DIVIDE_fn(DESCR_t a, DESCR_t b);
DESCR_t POWER_fn(DESCR_t a, DESCR_t b);
DESCR_t neg(DESCR_t a);
DESCR_t pos(DESCR_t a);
DESCR_t rt_cset_compl(DESCR_t a);
#ifdef __cplusplus
}
#endif
#endif
