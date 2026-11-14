#ifndef CORE_STRING_BUILTINS_API_H
#define CORE_STRING_BUILTINS_API_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t DUPL_fn(DESCR_t s, DESCR_t n);
DESCR_t REPLACE_fn(DESCR_t s, DESCR_t from, DESCR_t to);
DESCR_t SUBSTR_fn(DESCR_t s, DESCR_t i, DESCR_t n);
DESCR_t SUBSTR_bytes_fn(DESCR_t s, DESCR_t i, DESCR_t n);
DESCR_t TRIM_fn(DESCR_t s);
DESCR_t lpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t rpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t REVERS_fn(DESCR_t s);
DESCR_t BCHAR_fn(DESCR_t n);
int ident(DESCR_t a, DESCR_t b);
int differ(DESCR_t a, DESCR_t b);
#ifdef __cplusplus
}
#endif
#endif
