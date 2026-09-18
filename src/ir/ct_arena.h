/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef CT_ARENA_H
#define CT_ARENA_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
void  *ct_alloc(size_t n);
void  *ct_zalloc(size_t n, size_t sz);
void  *ct_grow(void *p, size_t n);
char  *ct_strdup(const char *s);
char  *ct_strndup(const char *s, size_t n);
void   ct_drop(void *p);
size_t ct_arena_bytes(void);
size_t ct_arena_mapped(void);
#ifdef __cplusplus
}
#endif
#endif
