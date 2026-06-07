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
    void **γ_site;
    void **ω_site;
} DTP_FRAG_t;
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t *g_pat_pool_base;
extern uint8_t *g_pat_pool_cur;
extern uint8_t *g_pat_pool_end;
void pat_pool_init(void);
void pat_pool_reset(void);
#endif
