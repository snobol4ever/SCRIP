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
    struct scrip_coctx_t *activator;  /* RUNG 4: who resumed this coexpression -- coret/cofail switch BACK here (mirrors k_current's activator chain). Set by RUNG 5's `@` before it switches in; NULL until then, so coret/cofail are correct-by-construction but unexercised until `@` lands (matches RUNG 1/2/3's built-not-yet-reachable rhythm). */
    void     *resume_addr;   /* RUNG 4: where the body resumes on the NEXT `@` -- coret LEAs its resumeLabel here (JCON's p.expr.ir.resume). RUNG 5 reads it to know where to re-enter. */
    int       dead;          /* RUNG 4: cofail sets this permanently; RUNG 5's `@` reads it to fail immediately on a resume of an exhausted coexpression (JCON's vCoexp exhaustion). */
    uint64_t  xmit[2];       /* RUNG 4: the 16-byte DESCR the body just produced -- coret stores it here so the activator's `@`-expression can read the transmitted value (JCON stores it in the coexpr object). */
} scrip_coctx_t;

void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first);
void scrip_coexpr_destroy(scrip_coctx_t *ctx);
extern scrip_coctx_t *scrip_co_current;  /* RUNG 4: the coexpression whose body is currently executing (JCON's k_current, one layer above the switch primitive per rt_coexpr.c's own header note). Set by the trampoline before it jumps into a body; read by scrip_coret/scrip_cofail. */
void scrip_coret(uint64_t d0, uint64_t d1, void *resume_addr);  /* RUNG 4: body yields the DESCR {d0,d1}, remembers resume_addr, switches back to scrip_co_current->activator. */
void scrip_cofail(void);  /* RUNG 4: body exhausted -- mark scrip_co_current dead, switch back to its activator. */

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
