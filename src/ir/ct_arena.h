/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef CT_ARENA_H
#define CT_ARENA_H
#include <stddef.h>
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif
    void *ct_alloc(size_t n);
    void *ct_zalloc(size_t n, size_t sz);
    void *ct_grow(void *p, size_t n);
    char *ct_strdup(const char *s);
    char *ct_strndup(const char *s, size_t n);
    void ct_drop(void *p);
    size_t ct_arena_bytes(void);
    size_t ct_arena_mapped(void);
    long ct_arena_scan(const char *lo, const char *hi, void (*fn)(void *ctx, const char *plo, const char *phi), void *ctx);
    int vfmt_len(const char *fmt, va_list ap);
    int fmt_len(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
    char *ct_vfmt(const char *fmt, va_list ap);
    char *ct_fmt(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
#ifdef __cplusplus
}
#endif
#endif
