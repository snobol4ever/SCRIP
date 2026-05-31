#include "emit_io.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
static FILE *          g_emit_sink = NULL;
void emit_io_set_sink(FILE * out) { g_emit_sink = out; }
FILE * emit_io_get_sink(void)     { return g_emit_sink; }
void emit_text_n(const char * s, size_t n) {
    if (!s || n == 0) return;
    if (g_emit_sink) fwrite(s, 1, n, g_emit_sink);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_textf(const char * fmt, ...) {
    if (!fmt || !g_emit_sink) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_emit_sink, fmt, ap);
    va_end(ap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_1asm(const char * a)                 { emit_textf(" %s\n", a); }
void emit_2asm(const char * a, const char * b) { emit_textf(" %s %s\n", a, b); }
void emit_directive(const char * line)         { emit_textf("%s\n", line); }
void emit_comment(const char * line)           { emit_textf("%s\n", line); }
size_t emit_io_flush(FILE * out) { (void)out; return 0; }
void   emit_io_reset(void)       { }
