#ifndef BOX_RT_H
#define BOX_RT_H
#include <stdint.h>
#include "gen.h"
#include "IR.h"
typedef struct {
    int64_t from_i; int64_t to_i; int64_t by_i; int64_t cur_i;
    double  from_r; double  to_r; double  by_r; double  cur_r;
    int is_real; int started;
} to_by_rt_state_t;
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
