#ifndef SNOBOL4_H
#define SNOBOL4_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
int lex_cmp_pair(DESCR_t a, DESCR_t b);
DESCR_t rt_raise(long cls, long code, const char *text, DESCR_t value, long exit_code);
DESCR_t rt_raise_line(long cls, long code, const char *line, long exit_code);
void rt_root_ball_exit(void) __attribute__((noreturn));
void rt_ball_rule_set(long by_code);
const char *rt_sno_indirect_name(DESCR_t v);
int is_numeric_like(DESCR_t d);
double to_real(DESCR_t v);
const char *datatype(DESCR_t v);
DESCR_t input_read(void);
DESCR_t terminal_read(void);
extern const char alphabet[257];
void core_lib_init(void);
int rt_name_textless(DESCR_t v);
int rt_str_arg_bad(DESCR_t d);
int64_t rt_time_ns(void);
#ifdef __cplusplus
}
#endif
#endif
