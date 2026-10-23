/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef KEYWORDS_H
#define KEYWORDS_H
#include <limits.h>
#include "g_runtime.h"
#include "../ir/descr.h"
#define G_ERROR_EVAL_STAGE LONG_MIN
#define G_ERROR_EVAL_LEVELS (1L << 24)
#define G_ERROR_EVAL_AT(l) (G_ERROR_EVAL_STAGE + ((long)(l) & (G_ERROR_EVAL_LEVELS - 1)))
#define G_ERROR_EVAL_ERRED_AT(l) (G_ERROR_EVAL_AT(l) + G_ERROR_EVAL_LEVELS)
#define G_ERROR_IS_EVAL(e) ((long)(e) >= G_ERROR_EVAL_STAGE && (long)(e) < G_ERROR_EVAL_STAGE + 2 * G_ERROR_EVAL_LEVELS)
#define G_ERROR_EVAL_ERRED(e) ((long)(e) >= G_ERROR_EVAL_STAGE + G_ERROR_EVAL_LEVELS && (long)(e) < G_ERROR_EVAL_STAGE + 2 * G_ERROR_EVAL_LEVELS)
#define G_ERROR_EVAL_LEVEL(e) (((long)(e) - G_ERROR_EVAL_STAGE) & (G_ERROR_EVAL_LEVELS - 1))
int kw_assign(const char *kw, DESCR_t val);
int kw_can_assign(const char *kw, DESCR_t val);
DESCR_t kw_read(const char *kw);
DESCR_t rt_keyword_read(const char *sval);
DESCR_t rt_keyword_read_snobol4(const char *sval);
DESCR_t rt_keyword_gen(const char *sval, long idx);
int rt_kw_direct_on(void);
const char *rt_kw_direct_sym(int idx, int *soff, const void **base);
void rt_kw_seed_defaults(void);
int rt_kw_index(const char *kw);
DESCR_t rt_kw_read_idx(int64_t idx);
DESCR_t rt_kw_write_idx(int64_t idx, DESCR_t v);
const char *kw_cset_name(const char *ptr);
int kw_cset_len(const char *ptr);
const char *kw_cset_const_str(const char *kw);
void rt_kw_publish_error(int code, const char *msg);
void rt_kw_dump_values(void (*emit)(void *cx, const char *name, DESCR_t v), void *cx);
#endif
