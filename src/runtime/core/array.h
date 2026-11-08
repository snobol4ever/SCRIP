#ifndef ARRAY_H
#define ARRAY_H
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _ARBLK_t { int lo, hi; int ndim; int lo2, hi2; int proto_bare; const char *proto; DESCR_t *data; long id; long dumpno; int ewidth; int esigned; } ARBLK_t;
long rt_sno_dumpno_next(void);
void rt_sno_dumpno_undo(void);
long rt_agg_serial_list(void);
long rt_agg_serial_table(void);
long rt_agg_serial_set(void);
ARBLK_t *array_new(int lo, int hi);
ARBLK_t *array_new2d(int lo1, int hi1, int lo2, int hi2);
DESCR_t array_get(ARBLK_t *a, int i);
void array_set(ARBLK_t *a, int i, DESCR_t v);
DESCR_t array_get2(ARBLK_t *a, int i, int j);
void array_set2(ARBLK_t *a, int i, int j, DESCR_t v);
DESCR_t *array_ptr(ARBLK_t *a, int i);
#define ARRAY_VAL(a_) ((DESCR_t){ .v = DT_A, .arr = (a_) })
#ifdef __cplusplus
}
#endif
#endif
