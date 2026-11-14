#ifndef CORE_PATTERN_MATCH_API_H
#define CORE_PATTERN_MATCH_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
DESCR_t c_rt_subscript_var(DESCR_t base, DESCR_t idx);
DESCR_t c_rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
DESCR_t c_rt_subscript_var2(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
DESCR_t c_rt_subscript_var2_lv(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
DESCR_t c_rt_assign_var(DESCR_t var, DESCR_t val);
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end);
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
void register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args);
DESCR_t EVAL_fn(DESCR_t expr);
DESCR_t opsyn(DESCR_t newname, DESCR_t oldname, DESCR_t type);
DESCR_t sort_fn(DESCR_t arr, DESCR_t col);
DESCR_t rsort_fn(DESCR_t arr, DESCR_t col);
#ifdef __cplusplus
}
#endif
#endif
