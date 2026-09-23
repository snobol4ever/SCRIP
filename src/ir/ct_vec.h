/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef CT_VEC_H
#define CT_VEC_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ct_arena.h"
typedef struct { void *p; uint32_t len, cap, esz; } cv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void cv_reserve(cv_t *v, uint32_t esz, uint64_t need, const char *name) {
    uint64_t nc;
    if (need <= v->cap) return;
    nc = v->cap ? (uint64_t)v->cap * 2 : 8;
    if (nc < need) nc = need;
    if (nc > UINT32_MAX || esz == 0) { fprintf(stderr, "ct_vec: table %s cannot grow to %llu elements of %u bytes\n", name, (unsigned long long)nc, esz); _exit(3); }
    v->p = ct_grow(v->p, (size_t)(nc * esz));
    v->cap = (uint32_t)nc;
    v->esz = esz;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void *cv_push(cv_t *v, uint32_t esz, const char *name) {
    void *e;
    cv_reserve(v, esz, (uint64_t)v->len + 1, name);
    e = (char *)v->p + (size_t)v->len * esz;
    memset(e, 0, esz);
    v->len++;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CV_AT(v, T, i) (((T *)(v).p)[i])
#define CV_PUSH(v, T)  (*(T *)cv_push(&(v), (uint32_t)sizeof(T), #v))
#endif
