#ifndef EMITTER_EMIT_DATA_H
#define EMITTER_EMIT_DATA_H
#include <stddef.h>
#include "ir/ct_vec.h"
#ifdef __cplusplus
extern "C" {
#endif
void strtab_label(char *buf, size_t bufsz, const char *s);
void xa_emit_strtab_rodata(void);
void strtab_reset(void);
int strtab_intern(const char *s);
const void *csettab_label(char *buf, size_t bufsz, const char *cset);
void xa_emit_csettab_rodata(void);
void csettab_reset(void);
extern cv_t g_flat_data_buf;
extern size_t g_flat_data_len;
extern int g_flat_data_any;
void data_buf_flush_pending_label(void);
void data_buf_reset(void);
#ifdef __cplusplus
}
#endif
#endif
