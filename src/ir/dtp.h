/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DTP_H
#define DTP_H
#include <stdint.h>
void *dtp_wrap_fn_sz(void *fn, int64_t zsz, int32_t zstatic);
int64_t dtp_zsz_of(void *headv);
int dtp_zstatic_of(void *headv);
#endif
