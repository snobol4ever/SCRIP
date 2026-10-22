#ifndef G_RUNTIME_BN_H
#define G_RUNTIME_BN_H
#include "descr.h"
#include "ct_vec.h"
#define RT_TWEAK_IC_N 64
#define RT_CTOR_IC_N 256
#define RT_FIELD_IC_N 256
struct _DATINST_tType;
struct g_tweak_slot { const char *cname; long gen; };
struct g_ctor_slot { const char *fn; void *dt; long gen; };
struct g_field_slot { const char *fn; const struct _DATINST_tType *type; int idx; };
typedef struct { unsigned gen; unsigned char len; signed char kind; short nf; char dxn[14]; void *ctor; const char *syn; } g_dtax_ent_t;
typedef struct {
    cv_t gram;
    long tweak_gen_bump;
    const char *pl_edin_out_name;
    const char *pl_edin_in_name;
    cv_t g_pl_flags;
    const char *ctor_ic_site;
    DESCR_t main_args_descr;
    char **main_args_v;
    int main_args_n;
    const char *main_progname;
    g_dtax_ent_t dtax[256];
    g_dtax_ent_t dtax_bid[1025];
    long bidprof[1024];
    int bidprof_on;
    int bidjmp_on;
    cv_t rt_syn;
    struct g_tweak_slot tweak_none[RT_TWEAK_IC_N];
    struct g_ctor_slot ctor_ic[RT_CTOR_IC_N];
    struct g_field_slot field_ic[RT_FIELD_IC_N];
} g_bn_t;
#endif
