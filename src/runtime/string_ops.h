/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef STRING_OPS_H
#define STRING_OPS_H
#include "../contracts/descr.h"
DESCR_t     str_concat_d(DESCR_t a, DESCR_t b);
DESCR_t     c_str_concat_d(DESCR_t a, DESCR_t b);
DESCR_t     str_concat_fracdigit_d(DESCR_t a, DESCR_t b);
DESCR_t     str_repeat_d(DESCR_t s, DESCR_t n);
DESCR_t     lconcat_d(DESCR_t a, DESCR_t b);
const char *real_str(double r, char *buf, int bufsz);
const char *icon_real_str(double r, char *buf, int bufsz);
#endif
