/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZETA_HEAP_H
#define ZETA_HEAP_H
unsigned rt_zh_alloc(long bytes, void **ubout);
void    *rt_zh_deref(unsigned h);
void     rt_zh_pin(unsigned h);
void     rt_zh_unpin(unsigned h);
void     rt_zh_mark_dead(unsigned h);
unsigned rt_zh_birthmark(void);
void     rt_zh_kill_since(unsigned mark);
long     rt_zh_live_count(void);
#endif
