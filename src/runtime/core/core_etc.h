#ifndef CORE_ETC_H
#define CORE_ETC_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
void rt_translate_bytes(char *dst, const char *src, size_t n, const char *map);
DESCR_t INVOKE_fn(const char *name, DESCR_t *args, int nargs);
DESCR_t dat_field_call(const char *name, DESCR_t *args, int nargs);
DESCR_t ARGVAL_fn(DESCR_t d);
DESCR_t INTVAL_fn(DESCR_t d);
DESCR_t PATVAL_fn(DESCR_t d);
DESCR_t VARVUP_fn(DESCR_t d);
char *STRCONCAT_fn(const char *a, const char *b);
DESCR_t CONCAT_fn(DESCR_t a, DESCR_t b);
char *STRDUP_fn(const char *s);
int64_t size(const char *s);
void PUSH_fn(DESCR_t v);
DESCR_t POP_fn(void);
DESCR_t TOP_fn(void);
int STACK_DEPTH_fn(void);
DESCR_t SIZE_fn(DESCR_t s);
DESCR_t INTGER_fn(DESCR_t v);
DESCR_t real_fn(DESCR_t v);
DESCR_t string_fn(DESCR_t v);
int eq(DESCR_t a, DESCR_t b);
int ne(DESCR_t a, DESCR_t b);
int val_stack_depth(void);
DESCR_t CODE_fn(DESCR_t str_d);
void indirect_goto(const char *varname);
#ifdef __cplusplus
}
#endif
#endif
