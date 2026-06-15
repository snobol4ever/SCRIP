/* ATTIC: dead code excised from src/emitter/emit_io.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
FILE * emit_io_get_sink(void)     { return g_emit_sink; }
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_textf(const char * fmt, ...) {
    if (!fmt || !g_emit_sink) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_emit_sink, fmt, ap);
    va_end(ap);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_1asm(const char * a)                 { emit_textf(" %s\n", a); }
void emit_2asm(const char * a, const char * b) { emit_textf(" %s %s\n", a, b); }
void emit_directive(const char * line)         { emit_textf("%s\n", line); }
void emit_comment(const char * line)           { emit_textf("%s\n", line); }
size_t emit_io_flush(FILE * out) { (void)out; return 0; }
void   emit_io_reset(void)       { }
