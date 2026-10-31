#ifndef LC_VEC_H
#define LC_VEC_H
typedef struct { void * data; int n; int cap; int esz; } lc_vec;
void lc_vec_init(lc_vec * v, int esz);
void * lc_vec_push(lc_vec * v, const void * elem);
void * lc_vec_at(const lc_vec * v, int i);
#define LC_AT(v, T, i) (((T *)(v)->data)[i])
#endif
