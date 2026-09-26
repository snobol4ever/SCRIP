#ifndef ICN_ACT_H
#define ICN_ACT_H
typedef struct icn_act_rec { const char *name; void *base; int np; long line; const char *file; } icn_act_rec_t;
#define ICN_ACT_CAP (1 << 16)
#ifdef __cplusplus
extern "C" {
#endif
extern icn_act_rec_t g_icn_act[ICN_ACT_CAP];
#ifdef __cplusplus
}
#endif
#endif
