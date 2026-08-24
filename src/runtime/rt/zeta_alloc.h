/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZETA_ALLOC_H
#define ZETA_ALLOC_H
void *rt_zls_alloc(long bytes);
void  rt_zls_release(void *fb);
void *rt_zls_mark(void);
void  rt_zls_release_to(void *mark);
void  rt_zls_arbno_step1_store(void *p);
void *rt_zls_arbno_step1_load(void);
void *rt_zls2_init(void);
void *rt_zls2_lo(void);
void *rt_zls2_push(long k);
void  rt_zls2_pop(long k);
void *rt_zls2_mark(void);
extern char *g_zls2_cur;
void  rt_zls2_release_to(void *mark);
int  rt_zeta_mode(void);
int  rt_zeta_storage_get(void);
int  rt_zc_frame_live(void);
int  rt_zeta_port_mode(void);
#endif
