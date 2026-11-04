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
char *VARVAL_fn(DESCR_t v);
int lex_cmp_pair(DESCR_t a, DESCR_t b);
DESCR_t rt_raise(long cls, long code, const char *text, DESCR_t value, long exit_code);
DESCR_t rt_raise_line(long cls, long code, const char *line, long exit_code);
void rt_root_ball_exit(void) __attribute__((noreturn));
void rt_ball_rule_set(long by_code);
void rt_translate_bytes(char *dst, const char *src, size_t n, const char *map);
void c_rt_translate_bytes(char *dst, const char *src, size_t n, const char *map);
DESCR_t INVOKE_fn(const char *name, DESCR_t *args, int nargs);
DESCR_t dat_field_call(const char *name, DESCR_t *args, int nargs);
DESCR_t ARGVAL_fn(DESCR_t d);
DESCR_t VARVAL_d_fn(DESCR_t d);
const char *rt_sno_indirect_name(DESCR_t v);
DESCR_t INTVAL_fn(DESCR_t d);
DESCR_t PATVAL_fn(DESCR_t d);
DESCR_t VARVUP_fn(DESCR_t d);
static inline __attribute__((always_inline)) int rt_plain_int_str(const char *s) {
    if (!s) return 0;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '+' || *s == '-') s++;
    if (*s < '0' || *s > '9') return 0;
    while (*s >= '0' && *s <= '9') s++;
    while (*s == ' ' || *s == '\t') s++;
    return *s == '\0';
}
int is_numeric_like(DESCR_t d);
int64_t to_int(DESCR_t v);
double to_real(DESCR_t v);
char *STRCONCAT_fn(const char *a, const char *b);
DESCR_t CONCAT_fn(DESCR_t a, DESCR_t b);
char *STRDUP_fn(const char *s);
int64_t size(const char *s);
const char *datatype(DESCR_t v);
void PUSH_fn(DESCR_t v);
DESCR_t POP_fn(void);
DESCR_t TOP_fn(void);
int STACK_DEPTH_fn(void);
typedef struct { FILE *fp; char *varname; int is_output; int is_popen; char *buf; size_t cap; long rlen; int unit; char *prebind; } io_chan_t;
DESCR_t SIZE_fn(DESCR_t s);
DESCR_t DUPL_fn(DESCR_t s, DESCR_t n);
DESCR_t REPLACE_fn(DESCR_t s, DESCR_t from, DESCR_t to);
DESCR_t SUBSTR_fn(DESCR_t s, DESCR_t i, DESCR_t n);
DESCR_t SUBSTR_bytes_fn(DESCR_t s, DESCR_t i, DESCR_t n);
DESCR_t TRIM_fn(DESCR_t s);
DESCR_t lpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t rpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t REVERS_fn(DESCR_t s);
DESCR_t BCHAR_fn(DESCR_t n);
DESCR_t INTGER_fn(DESCR_t v);
DESCR_t real_fn(DESCR_t v);
DESCR_t string_fn(DESCR_t v);
int eq(DESCR_t a, DESCR_t b);
int ne(DESCR_t a, DESCR_t b);
int lt(DESCR_t a, DESCR_t b);
int le(DESCR_t a, DESCR_t b);
int gt(DESCR_t a, DESCR_t b);
int ge(DESCR_t a, DESCR_t b);
int ident(DESCR_t a, DESCR_t b);
int differ(DESCR_t a, DESCR_t b);
DESCR_t add(DESCR_t a, DESCR_t b);
DESCR_t sub(DESCR_t a, DESCR_t b);
DESCR_t mul(DESCR_t a, DESCR_t b);
DESCR_t DIVIDE_fn(DESCR_t a, DESCR_t b);
DESCR_t POWER_fn(DESCR_t a, DESCR_t b);
DESCR_t neg(DESCR_t a);
DESCR_t pos(DESCR_t a);
void output_val(DESCR_t v);
DESCR_t input_read(void);
DESCR_t terminal_read(void);
void output_str(const char *s);
int rt_proc_is_defined(const char *name);
extern const char alphabet[257];
void core_lib_init(void);
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
DESCR_t c_rt_subscript_var(DESCR_t base, DESCR_t idx);
DESCR_t c_rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
DESCR_t c_rt_subscript_var2(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
DESCR_t c_rt_subscript_var2_lv(DESCR_t base, DESCR_t idx1, DESCR_t idx2);
DESCR_t rt_deref(DESCR_t d);
DESCR_t c_rt_assign_var(DESCR_t var, DESCR_t val);
DESCR_t rt_cset_compl(DESCR_t a);
int rt_name_textless(DESCR_t v);
int rt_str_arg_bad(DESCR_t d);
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end);
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
int val_stack_depth(void);
void register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args);
int64_t rt_time_ns(void);
DESCR_t EVAL_fn(DESCR_t expr);
DESCR_t EXPVAL_fn(DESCR_t expr_d);
#define RT_CONVE_CHAIN_MARK 0xFFFFFFFDu
#define RT_TRY_EVAL_OPEN 3L
#define RT_RQ_EVAL_ARM 1L
int rt_eval_open_takes(DESCR_t a);
typedef struct { DESCR_t saved; DESCR_t key; DESCR_t fn; DESCR_t mark; DESCR_t built; DESCR_t esv; DESCR_t flags; } eval_frame_t;
DESCR_t CONVE_fn(DESCR_t str_d);
DESCR_t CODE_fn(DESCR_t str_d);
DESCR_t opsyn(DESCR_t newname, DESCR_t oldname, DESCR_t type);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t opsyn2(DESCR_t a, DESCR_t b) { return opsyn(a, b, NULVCL); }
DESCR_t sort_fn(DESCR_t arr, DESCR_t col);
DESCR_t rsort_fn(DESCR_t arr, DESCR_t col);
void indirect_goto(const char *varname);
#ifdef __cplusplus
}
#endif
#endif
