#ifndef ZETA_ALLOC_H
#define ZETA_ALLOC_H
void *rt_zls_alloc(long bytes);
void  rt_zls_release(void *fb);
void *rt_zls_mark(void);
void  rt_zls_release_to(void *mark);
/* BB-OWNED-ζ STEP 1 (Lon pivot, this session) — see zeta_alloc.c for full scope/limitation notes. */
void  rt_zls_arbno_step1_store(void *p);
void *rt_zls_arbno_step1_load(void);
/* ZLS2 ARENA (Lon directive 2026-07-08) — the DOWN-GROWING bare-bump ζ arena for emitted-code frame
 * discipline: no headers, no per-alloc runtime call; α/ω are `sub/add r12, K` in emitted code.  init is
 * idempotent and returns the initial frame pointer (the HIGH end of the reserve); lo() is the low bound
 * for overflow diagnostics. */
void *rt_zls2_init(void);
void *rt_zls2_lo(void);
void *rt_zls2_push(long k);
void  rt_zls2_pop(long k);
void *rt_zls2_mark(void);
extern char *g_zls2_cur;
void  rt_zls2_release_to(void *mark);
/* ZETA SUBSYSTEM SELECTOR (Lon directive 2026-07-09) — see zeta_choices.h ZC_ZETA block. */
void rt_zeta_set_mode(int m);
int  rt_zeta_mode(void);
/* ZETA PORT SELECTOR (Lon directive 2026-07-10) — the ZC_PORT axis (--zeta-port flag); see zeta_alloc.c. */
void rt_zeta_port_set_mode(int m);
void rt_zeta_storage_set(int s);
int  rt_zeta_storage_get(void);
int  rt_zc_frame_live(void);
int  rt_zeta_port_mode(void);
#endif
