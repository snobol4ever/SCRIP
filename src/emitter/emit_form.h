#ifndef EMIT_FORM_H
#define EMIT_FORM_H
#include "emit_core.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
extern int g_is_text;
extern int g_emit_text_mode;
extern int g_emit_pos;
typedef int emitter_t;
void emitter_init_binary    (bb_buf_t buf, int size);
void emitter_init_text      (FILE * out, int mode);
FILE * emitter_text_out     (void);
int    emitter_pos          (void);
int    emitter_end          (void);
void emit_label_define_bb    (bb_label_t * lbl);
#endif
