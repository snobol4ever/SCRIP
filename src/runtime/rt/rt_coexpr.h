/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_COEXPR_H
#define RT_COEXPR_H
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct scrip_coctx_t {
    pthread_t thread;
    sem_t     sema;
    sem_t    *semp;
    int       alive;
    void    (*entry_fn)(void *arg);
    void     *entry_arg;
    struct scrip_coctx_t *activator;
    void     *resume_addr;
    int       dead;
    uint64_t  xmit[2];
    void *stk_win; unsigned long stk_guard;
    struct scrip_coctx_t *gc_next; uint64_t gc_spill[6];
    void *frame_copy; uint64_t frame_copy_sz;
    void *scan_state;
    uint64_t rtcc_spill[32];   /* RTCC Option-B block-swap: 256B copy of g_rtcc_block at coswitch; zero-init = safe when SCRIP_RTCC=0 (rtcc_coexpr_save is a no-op) */
} scrip_coctx_t;
void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first);
void scrip_coexpr_destroy(scrip_coctx_t *ctx);
extern scrip_coctx_t *scrip_co_current;
void scrip_coret(uint64_t d0, uint64_t d1, void *resume_addr);
void scrip_cofail(void);
scrip_coctx_t *scrip_coexpr_create(void *body_entry_addr, const uint64_t regs[6], uint64_t frame_bytes);
int scrip_coexpr_activate(scrip_coctx_t *target, uint64_t x0, uint64_t x1, uint64_t *out2);
scrip_coctx_t *scrip_co_gc_head(void);
scrip_coctx_t *scrip_co_gc_root(void);
void scrip_co_ctx_init(scrip_coctx_t *ctx, void (*entry_fn)(void *), void *entry_arg);
void scrip_co_gc_link(scrip_coctx_t *ctx);
int scrip_co_main_known(pthread_t *out);
int scrip_co_stack_of(scrip_coctx_t *ctx, char **lo, char **hi);
#ifdef __cplusplus
}
#endif
#endif
