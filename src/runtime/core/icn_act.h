#ifndef ICN_ACT_H
#define ICN_ACT_H
#include "ct_vec.h"
typedef struct icn_act_rec { const char *name; void *base; int np; long line; const char *file; void *args; } icn_act_rec_t;
#define ICN_ACT(lv) CV_AT(g_icn_act, icn_act_rec_t, (lv))
#ifdef __cplusplus
extern "C" {
#endif
extern cv_t g_icn_act;
void rt_icn_act_reserve(int lv);
#ifdef __cplusplus
}
#endif
#endif
