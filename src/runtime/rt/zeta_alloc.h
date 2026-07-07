#ifndef ZETA_ALLOC_H
#define ZETA_ALLOC_H
void *rt_zls_alloc(long bytes);
void  rt_zls_release(void *fb);
/* BB-OWNED-ζ STEP 1 (Lon pivot, this session) — see zeta_alloc.c for full scope/limitation notes. */
void  rt_zls_arbno_step1_store(void *p);
void *rt_zls_arbno_step1_load(void);
#endif
