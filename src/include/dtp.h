#ifndef DTP_H
#define DTP_H
#include <stdint.h>
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct _DTP_t {
    void *entry;
    void *out_γ;
    void *out_ω;
} DTP_t;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct _DTP_FRAG_t {
    void  *entry;
    void  *β;
    void **γ_site;
    void **ω_site;
} DTP_FRAG_t;
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct _DTP_PROTO_DESC {
    int32_t entry_off;
    int32_t β_off;
    int32_t γ_off;
    int32_t ω_off;
    int32_t abort_off;
    int32_t op1_off;
    int32_t op2_off;
} DTP_PROTO_DESC;
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t *g_pat_pool_base;
extern uint8_t *g_pat_pool_cur;
extern uint8_t *g_pat_pool_end;
void pat_pool_init(void);
void pat_pool_reset(void);
DTP_FRAG_t rt_pattern_build(const void *proto, uint32_t len, const DTP_PROTO_DESC *desc, long op_i, const char *op_s);
void rt_pattern_stitch_cat(DTP_FRAG_t *out, const DTP_FRAG_t *l, const DTP_FRAG_t *r);
void rt_pattern_stitch_alt(DTP_FRAG_t *out, const DTP_FRAG_t *l, const DTP_FRAG_t *r);
#endif
