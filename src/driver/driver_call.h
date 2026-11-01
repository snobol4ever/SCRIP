#ifndef DRIVER_CALL_H
#define DRIVER_CALL_H
#include "ir/ct_vec.h"
#include "ir/descr.h"
typedef struct { const char *nm; DESCR_t val; } InitSlot;
typedef struct { int id; int ns; InitSlot *s; } InitEnt;
extern cv_t init_tab_v;
#define init_tab ((InitEnt *)init_tab_v.p)
extern int init_n;
void init_update_snapshot(char **snames, DESCR_t *svals, int nsaved);
DESCR_t call_user_function(const char *fname, DESCR_t *args, int nargs);
#endif
