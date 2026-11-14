#ifndef KEYWORDS_ETC_H
#define KEYWORDS_ETC_H
#include <limits.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
int kw_assign(const char *kw, DESCR_t val);
int kw_can_assign(const char *kw, DESCR_t val);
#ifdef __cplusplus
}
#endif
#endif
