#ifndef GC_HEAP_GC_ROOTS_ARGS_H
#define GC_HEAP_GC_ROOTS_ARGS_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct gc_roots_args_t { void **vbk; long vnb; void **fbk; long fnb; const char *udef; long pos_t; const char *pos_e; long pos_b; const char *tmp_end; const char *tmp_p; } gc_roots_args_t;
#ifdef __cplusplus
}
#endif
#endif
