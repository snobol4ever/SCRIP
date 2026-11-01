#ifndef EMITTER_EMIT_MEDIUM_H
#define EMITTER_EMIT_MEDIUM_H
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "ir/bb_pool.h"
#ifdef __cplusplus
extern "C" {
#endif
#define TEXT_MODE_INVOCATION 0
typedef enum { EMIT_TEXT = 0, EMIT_BINARY_WIRED = 1, EMIT_MACRO_DEF = 3, EMIT_TEXT_INLINE = 4, EMIT_JVM = 5, EMIT_JS = 6, EMIT_NET = 7, EMIT_WASM = 8 } bb_emit_mode_t;
typedef enum { BB_MEDIUM_TEXT = 0, BB_MEDIUM_BINARY = 1, BB_MEDIUM_MACRO_DEF = 2 } bb_medium_t;
extern bb_medium_t g_medium;
#define MEDIUM_TEXT (g_medium == BB_MEDIUM_TEXT)
#define MEDIUM_BINARY (g_medium == BB_MEDIUM_BINARY)
#define MEDIUM_MACRO_DEF (g_medium == BB_MEDIUM_MACRO_DEF)
extern bb_emit_mode_t bb_emit_mode;
extern int g_sm_native_unsupported;
extern FILE * bb_emit_out;
extern bb_buf_t bb_emit_buf;
extern int bb_emit_pos;
extern int bb_emit_size;
extern int g_is_text;
extern int g_emit_text_mode;
void bb_emit_begin (bb_buf_t buf, int size);
int bb_emit_end (void);
void bb_emit_byte (uint8_t b);
void bb_emit_bytes (const uint8_t * p, size_t k);
void bb_emit_u32 (uint32_t v);
void bb_emit_u64 (uint64_t v);
void bb_emit_i32 (int32_t v);
FILE * emit_outf (void);
void emitter_init_text (FILE * out, int mode);
void emitter_init_binary (bb_buf_t buf, int size);
int emitter_end (void);
extern int g_emit_pos;
void emit_text_n(const char * s, size_t n);
void emit_text_flush(void);
long emit_text_count(void);
void emit_textf (const char * fmt, ...) __attribute__((format(printf, 1, 2)));
void emit_textf_flush(void);
void emit_sep_rule_c(char ch);
void emit_set_sink(FILE * out);
extern int g_emit_module_pic;
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
#include <string>
void emit_text_s(const std::string & s);
void emit_write_file_s(const char * path, const std::string & s);
#endif
#endif
