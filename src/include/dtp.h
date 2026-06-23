#ifndef DTP_H
#define DTP_H
#include <stdint.h>
/*--------------------------------------------------------------------------------------------------------------------*/
extern uint8_t *g_pat_pool_base;
extern uint8_t *g_pat_pool_cur;
extern uint8_t *g_pat_pool_end;
void pat_pool_init(void);
void pat_pool_reset(void);
#endif
