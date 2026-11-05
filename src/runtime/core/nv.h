#ifndef NV_H
#define NV_H
#include <stdint.h>
#include <stddef.h>
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
DESCR_t NV_GET_fn(const char *name);
DESCR_t NV_SET_fn(const char *name, DESCR_t val);
void NV_CLEAR_fn(const char **except, int nexcept);
typedef struct { const char *name; DESCR_t val; } NvPair;
int nv_snapshot(NvPair **out);
void nv_restore(const NvPair *pairs, int n);
DESCR_t INDR_GET_fn(const char *name);
void INDR_SET_fn(const char *name, DESCR_t val);
DESCR_t NAME_fn(const char *varname);
int ASGNIC_fn(const char *kw_name, DESCR_t val);
void NAME_pop(void);
void NAME_commit(void);
typedef struct _VarEntry {
    char *name;
    uint32_t nlen;
    int has_nul;
    unsigned long serial;
    int std_detached;
    DESCR_t val;
    DESCR_t *cell;
    int is_gva;
    int is_const;
    int touched;
    int is_io;
    struct _VarEntry *next;
} NV_t;
DESCR_t *NV_CELL_PLAIN_fn(const char *name);
DESCR_t *NV_CELL_FAST_n(const char *s, size_t n);
DESCR_t *NV_PTR_fn(const char *name);
DESCR_t *NV_PTR_n(const char *s, size_t n);
const char *NV_nul_key(const char *s, size_t n);
int NV_name_needs_key(const char *s, size_t n);
__attribute__((visibility("hidden"))) DESCR_t *NV_CELL_IF_FASTSET_fn(const char *name);
int NV_bind_gva(const char *name, DESCR_t *cell);
int NV_EXISTS_fn(const char *name);
DESCR_t NV_KW_GET_fn(const char *name);
DESCR_t NV_KW_SET_fn(const char *name, DESCR_t val);
int NV_CONST_ASSIGNED_fn(const char *name);
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n, int first);
int rt_gva_first_named(const char **names, int n);
const char *NV_name_from_ptr(const DESCR_t *ptr);
#ifdef __cplusplus
}
#endif
#endif
