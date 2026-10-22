#ifndef G_RUNTIME_KW_H
#define G_RUNTIME_KW_H
#include <stdint.h>
struct g_kw_cset_ent;
typedef struct {
    int64_t kw_maxint;
    const char *lastfile;
    const char *sno_errtext;
    struct g_kw_cset_ent *kw_cset_names;
    int kw_cset_count;
    int kw_cset_cap;
    int *kw_cset_hidx;
    int kw_cset_hcap;
    int *kw_cset_cidx;
    long kw_cset_hidx_gen;
    const char *kw_cset_regc_ptr[64];
    int kw_cset_regc_len[64];
} g_kw_t;
#endif
