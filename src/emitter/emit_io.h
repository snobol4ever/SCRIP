#ifndef EMIT_IO_H
#define EMIT_IO_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdio.h>
void emit_text_n(const char * s, size_t n);
void emit_textf (const char * fmt, ...)
                 __attribute__((format(printf, 1, 2)));
void emit_1asm  (const char * a);
void emit_2asm  (const char * a, const char * b);
size_t emit_io_flush(FILE * out);
void   emit_io_reset(void);
void   emit_io_set_sink(FILE * out);
FILE * emit_io_get_sink(void);
#ifdef __cplusplus
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C++" {
#include <string>
inline void emit_1asm (const std::string & a)            { emit_1asm(a.c_str()); }
inline void emit_2asm (const std::string & a, const std::string & b) { emit_2asm(a.c_str(), b.c_str()); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#else
#endif
#endif
