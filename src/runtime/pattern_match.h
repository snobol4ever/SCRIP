/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PATTERN_MATCH_H
#define PATTERN_MATCH_H
#include "../ir/descr.h"
int cset_resolve(DESCR_t arg, const char **out_ptr, int *out_len);
int cset_has(const char *cv, int clen, unsigned char ch);
#endif
