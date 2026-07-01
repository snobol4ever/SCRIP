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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rt_coexpr.h"

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coctx_t -- one per live co-expression, OWNED BY THE CALLER (RUNG 3's bb_create.cpp allocates one;
 * this file never allocates or frees one itself, exactly as coswitch() never calls alloc() on a `context`
 * for anything other than ITS OWN bookkeeping fields -- the Icon reference's b_coexpr/refresh-block struct
 * is a much larger thing this small struct deliberately does not try to replicate; this struct is ONLY the
 * pthread+semaphore plumbing, nothing about SCRIP-level co-expression semantics). Layout now lives in
 * rt_coexpr.h (RUNG 3, Claude Sonnet 2026-07-01) so bb_create.cpp shares this exact definition rather
 * than a hand-duplicated copy that could silently drift out of sync.
 */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coexpr_entry_pkg_t -- RUNG 3 (Claude Sonnet, 2026-07-01). The payload scrip_co_trampoline's
 * entry_fn(entry_arg) receives: the creator's FULL register-contract state (not just r12 -- see this
 * commit's own comment above for why r13-r15/rbx/rbp are included conservatively) plus the resolved
 * body-entry code address (from bb_create.cpp's op_sval_lbl-derived LEA, passed in as a raw pointer).
 * A mid-chain flat-chain label assumes ALL SIX of these registers are already correctly loaded when
 * control reaches it (see xa_flat_prologue_str in xa_flat.cpp: r12 comes from a per-call `mov r12,rdi`
 * at the GLOB's true entry, r13-r15 are conditionally established per-call via g_emit_frame_caller_dl
 * -- neither is process-global or ambient) -- a bare `jmp` into that label from a brand-new pthread,
 * whose registers start from whatever the OS/pthread library initialized them to, would silently read
 * garbage through r12 et al. rather than crash. This package + the trampoline below exist specifically
 * to avoid that.
 */
typedef struct scrip_coexpr_entry_pkg_t {
    void    *body_entry_addr;   /* the resolved mid-chain label to jump to (op_sval_lbl's runtime address) */
    uint64_t r12, r13, r14, r15, rbx, rbp;  /* the creator's full register-contract state at create-time */
} scrip_coexpr_entry_pkg_t;
/* The trampoline's inline asm hardcodes these field displacements (it cannot use offsetof from inside an
   asm string). Assert them here so a field reorder is a COMPILE error, not silent register corruption. */
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, body_entry_addr) ==  0, "pkg layout drift: body_entry_addr");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r12)             ==  8, "pkg layout drift: r12");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r13)             == 16, "pkg layout drift: r13");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r14)             == 24, "pkg layout drift: r14");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r15)             == 32, "pkg layout drift: r15");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbx)             == 40, "pkg layout drift: rbx");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbp)             == 48, "pkg layout drift: rbp");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coexpr_trampoline_entry -- the entry_fn every RUNG-3-created scrip_coctx_t uses. Runs on the
 * new pthread, AFTER scrip_co_trampoline's own sem_wait unblocks it (i.e. only once this coexpression
 * is actually first resumed, matching co_init's "lay out state, don't run yet" semantics -- see this
 * file's own top-of-file comment and GOAL-IR-IMMUTABLE-EMIT.md's rswitch.c research). Restores the
 * full register contract from the package, then indirect-jumps to body_entry_addr -- from that point
 * on, execution is ordinary generated glob code and this function never returns to its own caller in
 * the normal sense (mirrors bb_to.cpp/every other generator template: control leaves via jmp, not ret).
 * Written in extended asm rather than emitted per-call from bb_create.cpp, because this logic is
 * IDENTICAL for every create-site in every program -- one compiled-once C function, not one hand-rolled
 * x86 stub per call site (the STUB bb_create.cpp emits is just "build the package, call this").
 */
void scrip_coexpr_trampoline_entry(void *arg) {
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)arg;
    /* Load all six contract registers from the package inside a single asm block that ENDS in the jmp
       and never returns. rbp is loaded LAST and is deliberately NOT an asm operand or clobber: GCC
       forbids listing rbp as a clobber/output when it's the frame pointer of the enclosing function
       ("bp cannot be used in 'asm' here"). Passing the package POINTER and dereferencing each field via
       explicit [pkg+offset] memory operands sidesteps that -- GCC never has to allocate or preserve rbp
       around the block, because by the time rbp is overwritten the very next instruction is the jmp out.
       Offsets are validated below by a compile-time assert against the actual struct layout, so a field
       reorder can't silently desync these hardcoded displacements from the C struct (the exact
       silent-drift risk this feature's own notes warn about). */
    __asm__ volatile (
        "mov  0(%0), %%rax\n\t"   /* pkg->body_entry_addr */
        "mov  8(%0), %%r12\n\t"   /* pkg->r12 */
        "mov 16(%0), %%r13\n\t"   /* pkg->r13 */
        "mov 24(%0), %%r14\n\t"   /* pkg->r14 */
        "mov 32(%0), %%r15\n\t"   /* pkg->r15 */
        "mov 40(%0), %%rbx\n\t"   /* pkg->rbx */
        "mov 48(%0), %%rbp\n\t"   /* pkg->rbp -- loaded last, immediately before the jmp */
        "jmp *%%rax\n\t"
        :
        : "r"(pkg)
        : "rax", "r12", "r13", "r14", "r15", "rbx", "memory"
    );
    /* unreachable: the jmp above never returns, exactly like scrip_co_trampoline's own entry_fn(arg)
       call is documented never to return (see that function's FATAL-abort comment). If we get here,
       the jmp itself faulted or target was garbage -- fail loud, not silently continue past this point. */
    fprintf(stderr, "scrip_coexpr: FATAL scrip_coexpr_trampoline_entry fell through the jmp -- bad body_entry_addr?\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * scrip_coexpr_create -- the C-side helper bb_create.cpp's template calls. Allocates ONE scrip_coctx_t
 * (heap, this call site's -- never freed by this file per the caller-owns contract; RUNG 3 does not yet
 * wire coexpression garbage collection, matching JCON's own vCoexp which is ordinary GC'd Java data and
 * has no explicit free path either) plus its entry package, wires entry_fn=scrip_coexpr_trampoline_entry,
 * and returns the coctx pointer for bb_create.cpp to store into its frame slot. Does NOT call
 * scrip_coswitch itself -- per RUNG 1's lowering (create succeeds immediately, body doesn't run until
 * the first `@`/resume) and co_init's semantics (RUNG 2's own research), creation and first-switch are
 * separate events; RUNG 5 (the `@`/resume side, not yet designed) is what will call scrip_coswitch.
 */
scrip_coctx_t *scrip_coexpr_create(void *body_entry_addr, const uint64_t regs[6]) {
    scrip_coctx_t *ctx = (scrip_coctx_t *)malloc(sizeof(scrip_coctx_t));
    if (!ctx) scrip_co_uerror("scrip_coexpr: malloc scrip_coctx_t failed");
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)malloc(sizeof(scrip_coexpr_entry_pkg_t));
    if (!pkg) scrip_co_uerror("scrip_coexpr: malloc scrip_coexpr_entry_pkg_t failed");
    pkg->body_entry_addr = body_entry_addr;
    pkg->r12 = regs[0]; pkg->r13 = regs[1]; pkg->r14 = regs[2];
    pkg->r15 = regs[3]; pkg->rbx = regs[4]; pkg->rbp = regs[5];
    ctx->entry_fn  = scrip_coexpr_trampoline_entry;
    ctx->entry_arg = pkg;
    ctx->alive = 0;   /* not yet started; scrip_coswitch's first==0 branch sets this to 1 on first switch */
    ctx->semp  = NULL;
    return ctx;
}
