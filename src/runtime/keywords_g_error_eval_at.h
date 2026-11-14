#ifndef KEYWORDS_G_ERROR_EVAL_AT_H
#define KEYWORDS_G_ERROR_EVAL_AT_H
#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif
#define G_ERROR_EVAL_STAGE LONG_MIN
#define G_ERROR_EVAL_LEVELS (1L << 24)
#define G_ERROR_EVAL_AT(l) (G_ERROR_EVAL_STAGE + ((long)(l) & (G_ERROR_EVAL_LEVELS - 1)))
#define G_ERROR_EVAL_ERRED_AT(l) (G_ERROR_EVAL_AT(l) + G_ERROR_EVAL_LEVELS)
#define G_ERROR_IS_EVAL(e) ((long)(e) >= G_ERROR_EVAL_STAGE && (long)(e) < G_ERROR_EVAL_STAGE + 2 * G_ERROR_EVAL_LEVELS)
#define G_ERROR_EVAL_ERRED(e) ((long)(e) >= G_ERROR_EVAL_STAGE + G_ERROR_EVAL_LEVELS && (long)(e) < G_ERROR_EVAL_STAGE + 2 * G_ERROR_EVAL_LEVELS)
#define G_ERROR_EVAL_LEVEL(e) (((long)(e) - G_ERROR_EVAL_STAGE) & (G_ERROR_EVAL_LEVELS - 1))
#define G_ERROR_EVAL_LOCAL (1L << 23)
#ifdef __cplusplus
}
#endif
#endif
