/*
 * rt_coexpr.h -- public interface to rt_coexpr.c's scrip_coswitch primitive (RUNG 2, pulled out to a
 * header in RUNG 3, Claude Sonnet 2026-07-01, so bb_create.cpp has ONE source of truth for the struct
 * layout instead of hand-duplicating it in the template TU -- a duplicated definition that drifts is
 * exactly the "silent corruption, not a loud crash" failure mode GOAL-IR-IMMUTABLE-EMIT.md's RUNG 2
 * risk note already flagged for this feature; a shared header makes drift a compile error instead.
 *
 * scrip_coctx_t is OWNED BY THE CALLER (see rt_coexpr.c's own header comment) -- this header declares
 * the layout and the three entry points; it allocates nothing.
 */
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
    int       alive;         /* set to 0 to signal the thread should exit rather than resume, mirrors rswitch.c's old->alive check */
    void    (*entry_fn)(void *arg);  /* the function this coexpression's thread should run, once first switched to */
    void     *entry_arg;
} scrip_coctx_t;

void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first);
void scrip_coexpr_destroy(scrip_coctx_t *ctx);

/* RUNG 3 (Claude Sonnet, 2026-07-01): allocates+wires a scrip_coctx_t for one `create EXPR` site.
   body_entry_addr = the resolved mid-chain label's runtime address (bb_create.cpp's LEA result).
   regs[6] = {r12,r13,r14,r15,rbx,rbp}, the creator's full register-contract state at create-time (see
   rt_coexpr.c's own scrip_coexpr_entry_pkg_t comment for why all six, not just r12). Packed as ONE
   array-pointer argument rather than 6 scalars specifically so bb_create.cpp's binary-mode call site
   stays within the SysV 6-integer-argument-register convention (rdi,rsi,rdx,rcx,r8,r9) and never needs
   to hand-encode a 7th stack-passed argument in raw x86 bytes -- unverifiable without a live disassemble/
   test cycle this session doesn't have. Does NOT call scrip_coswitch -- creation and first-resume are
   separate events, RUNG 1/RUNG 2's own established semantics. */
scrip_coctx_t *scrip_coexpr_create(void *body_entry_addr, const uint64_t regs[6]);

#ifdef __cplusplus
}
#endif

#endif /* RT_COEXPR_H */
