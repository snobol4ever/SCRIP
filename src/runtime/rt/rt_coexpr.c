/*
 * rt_coexpr.c -- SCRIP co-expression context-switch primitive (RUNG 2 of the co-expression feature,
 * GOAL-IR-IMMUTABLE-EMIT.md). A DELIBERATELY FAITHFUL PORT of refs/icon-master/src/common/rswitch.c's
 * coswitch(old, new, first) -- same pthread+semaphore strategy, same function shape, same caller-owns-the-
 * opaque-state-slots contract. A first attempt at this file (same session) deviated into a thread-local
 * "current coexpression" design under untested reasoning; that version was DELETED, not kept alongside this
 * one, after re-reading rswitch.c line by line found the reference doesn't need a thread-local OR a global
 * "current" pointer inside the switch primitive at all -- identity-tracking belongs one layer up, in
 * whatever calls scrip_coswitch (mirrors rcoexpr.r's co_chng owning `k_current`, one layer above
 * rswitch.c's coswitch, which is itself deliberately dumb and stateless except for lazy one-time init).
 *
 * scrip_coswitch is intentionally as dumb as coswitch(): given two opaque scrip_coctx_t* handles (old = the
 * caller's own context, new = the context to switch to) plus a `first` flag (0 the FIRST time `new` is
 * switched to, nonzero thereafter -- exactly rswitch.c's convention, kept identical on purpose), it wakes
 * `new` and blocks `old`, full stop. It does not know or care what a "co-expression" means, what value is
 * being transmitted, or who is "currently running" in any global sense -- that bookkeeping belongs to
 * RUNG 3/4 (bb_create.cpp / bb_coret.cpp / bb_cofail.cpp), which own an equivalent of rcoexpr.r's co_chng:
 * decide what to save/restore around the switch, then call this primitive to actually perform it.
 *
 * WHY PTHREADS INSTEAD OF A HAND-ROLLED REGISTER SAVE/RESTORE: see the file this replaced -- the reasoning
 * is unchanged (a co-expression's resume point can be reached from an arbitrary, unrelated call chain while
 * the original creating context keeps running independently in between; one OS thread per live
 * co-expression + a semaphore pair for mutual exclusion sidesteps hand-writing a stack-pointer swap because
 * the ORDINARY C function-call prologue/epilogue already preserves every callee-saved register across the
 * sem_wait() block point -- from the C compiler's point of view this is just a blocking function call).
 */

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coctx_t -- one per live co-expression, OWNED BY THE CALLER (RUNG 3's bb_create.cpp allocates one;
 * this file never allocates or frees one itself, exactly as coswitch() never calls alloc() on a `context`
 * for anything other than ITS OWN bookkeeping fields -- the Icon reference's b_coexpr/refresh-block struct
 * is a much larger thing this small struct deliberately does not try to replicate; this struct is ONLY the
 * pthread+semaphore plumbing, nothing about SCRIP-level co-expression semantics).
 */
typedef struct scrip_coctx_t {
    pthread_t thread;
    sem_t     sema;
    sem_t    *semp;
    int       alive;         /* set to 0 to signal the thread should exit rather than resume, mirrors rswitch.c's old->alive check */
    void    (*entry_fn)(void *arg);  /* the function this coexpression's thread should run, once first switched to */
    void     *entry_arg;
} scrip_coctx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int inited = 0;
static pthread_attr_t attribs;
static long g_coexp_stksize = 1024 * 1024; /* 1MB default; TODO RUNG 3+: no corpus program has yet needed this configurable, matches rswitch.c's `stksize` (COEXPSIZE) in spirit but not wired to a user-facing knob */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scrip_co_uerror(const char *msg) {
    perror(msg);
    fprintf(stderr, "scrip_coexpr: fatal error, aborting\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scrip_co_makesem(scrip_coctx_t *ctx) {
    if (sem_init(&ctx->sema, 0, 0) == -1) scrip_co_uerror("scrip_coexpr: sem_init failed");
    ctx->semp = &ctx->sema;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_co_trampoline -- runs in the new pthread. Blocks until the FIRST real switch-in wakes it (mirrors
 * rswitch.c's nctramp exactly: the thread exists but must not run entry_fn until explicitly resumed via the
 * semaphore, matching co_init's "lay out state, don't run yet" semantics from the Icon reference), then
 * calls entry_fn once. entry_fn is expected to itself call scrip_coswitch to hand control back (RUNG 4's
 * scrip_coret/scrip_cofail) as it produces values or exhausts -- this trampoline does not loop.
 */
static void *scrip_co_trampoline(void *arg) {
    scrip_coctx_t *self = (scrip_coctx_t *)arg;
    while (sem_wait(self->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in trampoline");
    self->entry_fn(self->entry_arg);
    fprintf(stderr, "scrip_coexpr: FATAL entry_fn returned to trampoline instead of switching away -- RUNG 3/4 bug\n");
    abort();
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coswitch(old, new, first) -- faithful mirror of rswitch.c's coswitch(old, new, first).
 * `old` = the CALLER's own context (must already exist -- allocated by whoever is running right now).
 * `new` = the context to switch TO.
 * `first` = 0 the very FIRST time `new` is ever switched to (triggers pthread_create + entry_fn setup via
 *           the trampoline); nonzero every subsequent switch into `new` (the thread already exists, just
 *           wake it). Same convention as the reference, kept identical on purpose -- RUNG 3/4 callers track
 *           "have I switched to this coctx before" exactly the way co_chng tracks it for the Icon reference.
 * Blocks the calling thread on `old`'s semaphore; wakes `new`'s thread. Returns once someone switches back
 * to `old` (i.e. this call does not return until CONTROL RETURNS to the caller -- ordinary blocking-call
 * semantics, nothing exotic).
 */
void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first) {
    if (!inited) {
        old->semp = &old->sema;
        scrip_co_makesem(old);
        old->thread = pthread_self();
        old->alive = 1;

        pthread_attr_init(&attribs);
        if (pthread_attr_setstacksize(&attribs, (size_t)g_coexp_stksize) != 0)
            scrip_co_uerror("scrip_coexpr: cannot set coexpression stack size");
        inited = 1;
    }

    if (first != 0) {
        /* new_ctx already has a running thread -- just wake it. */
    } else {
        scrip_co_makesem(new_ctx);
        new_ctx->alive = 1;
        if (pthread_create(&new_ctx->thread, &attribs, scrip_co_trampoline, new_ctx) != 0)
            scrip_co_uerror("scrip_coexpr: pthread_create failed");
    }

    sem_post(new_ctx->semp);
    while (sem_wait(old->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in scrip_coswitch");

    if (!old->alive) pthread_exit(NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coexpr_destroy(scrip_coctx_t *ctx) {
    ctx->alive = 0;
    sem_post(ctx->semp);
    pthread_join(ctx->thread, NULL);
    sem_destroy(ctx->semp);
}
