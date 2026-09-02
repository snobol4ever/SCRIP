#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>
#include "zeta_choices.h"
#include "gc_heap.h"
#include "rt_coexpr.h"
static int inited = 0;
static pthread_attr_t attribs;
static long g_coexp_stksize = 8L * 1024 * 1024;
scrip_coctx_t *scrip_co_current = NULL;
static scrip_coctx_t *g_co_gc_head = NULL;
static pthread_t g_co_main_thr;
static int g_co_main_set = 0;
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
static void *scrip_co_trampoline(void *arg) {
    scrip_coctx_t *self = (scrip_coctx_t *)arg;
    while (sem_wait(self->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in trampoline");
    scrip_co_current = self;
    self->entry_fn(self->entry_arg);
    fprintf(stderr, "scrip_coexpr: FATAL entry_fn returned to trampoline instead of switching away -- RUNG 3/4 bug\n");
    abort();
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first) {
    if (!inited) {
        old->semp = &old->sema;
        scrip_co_makesem(old);
        old->thread = pthread_self();
        old->alive = 1;
        g_co_main_thr = old->thread; g_co_main_set = 1;
        { const char *_cs = getenv("SCRIP_COEXP_STACK"); if (_cs && *_cs) { long _v = atol(_cs); if (_v >= (long)PTHREAD_STACK_MIN) g_coexp_stksize = _v; } }
        pthread_attr_init(&attribs);
        if (pthread_attr_setstacksize(&attribs, (size_t)g_coexp_stksize) != 0)
            scrip_co_uerror("scrip_coexpr: pthread_attr_setstacksize failed");
        inited = 1;
    }
    { extern void *rt_scan_state_capture(void *); old->scan_state = rt_scan_state_capture(old->scan_state); }
    if (first != 0) {
    } else {
        { extern void rt_scan_state_reset(void); rt_scan_state_reset(); }
        scrip_co_makesem(new_ctx);
        if (pthread_create(&new_ctx->thread, &attribs, scrip_co_trampoline, new_ctx) != 0)
            scrip_co_uerror("scrip_coexpr: pthread_create failed");
        { pthread_attr_t a; void *sa = 0; size_t sz = 0;   /* bounds only exist once the thread does -- alive is set AFTER this block, never before (row coexpr-stack-of-calls-pthread-getattr-np-on-an-uncreated-thread: alive=1 preceding thread creation is exactly what let scrip_co_stack_of reach pthread_getattr_np on a thread that did not exist yet). */
          if (pthread_getattr_np(new_ctx->thread, &a) != 0) scrip_co_uerror("scrip_coexpr: pthread_getattr_np on new thread failed");
          if (pthread_attr_getstack(&a, &sa, &sz) != 0) scrip_co_uerror("scrip_coexpr: pthread_attr_getstack on new thread failed");
          pthread_attr_destroy(&a);
          new_ctx->stk_lo = (char *)sa; new_ctx->stk_hi = (char *)sa + sz;
          rt_gc_root_range_add((const char *)new_ctx->stk_lo, (const char *)new_ctx->stk_hi); }
        new_ctx->alive = 1;
    }
    __asm__ volatile ("mov %%rbx,0(%0)\n\t.byte 0x48,0x89,0xe8\n\tmov %%rax,8(%0)\n\tmov %%r12,16(%0)\n\tmov %%r13,24(%0)\n\tmov %%r14,32(%0)\n\tmov %%r15,40(%0)\n\t" : : "r"(old->gc_spill) : "rax", "memory");   /* ⛔ the "rax" clobber is LOAD-BEARING (ceo s283h): the .byte trio is mov %rbp,%rax (rbp can't be named in a clobber list), so rax is destroyed mid-asm -- without declaring it, GCC is free to pick rax for %0, and the store "mov %rax,8(%0)" becomes mov %rax,8(%rax): it wrote rbp into [rbp+8], the caller's saved RETURN ADDRESS, and every coexpr activation returned into its own stack (SIGBUS at a stack rip, caught live by hardware watchpoint on this exact line). Latent until a build whose register allocator chose rax; first exercised end-to-end by the N-2 apply-call window. */
    { extern void rtcc_coexpr_save(uint64_t *); rtcc_coexpr_save(old->rtcc_spill); }
    sem_post(new_ctx->semp);
    while (sem_wait(old->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in scrip_coswitch");
    if (!old->alive) pthread_exit(NULL);
    { extern void rtcc_coexpr_restore(const uint64_t *); rtcc_coexpr_restore(old->rtcc_spill); }
    { extern void rt_scan_state_apply(void *); rt_scan_state_apply(old->scan_state); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coexpr_destroy(scrip_coctx_t *ctx) {
    ctx->alive = 0;
    sem_post(ctx->semp);
    pthread_join(ctx->thread, NULL);
    if (ctx->stk_lo) { rt_gc_root_range_del((const char *)ctx->stk_lo); ctx->stk_lo = 0; ctx->stk_hi = 0; }   /* pthread_join above already reclaimed the libc-mmap'd stack; nothing SCRIP-side to unprotect or refill. */
    { extern long g_scrip_coexpr_live; scrip_coctx_t **pp = &g_co_gc_head; while (*pp && *pp != ctx) pp = &(*pp)->gc_next; if (*pp) { *pp = ctx->gc_next; g_scrip_coexpr_live--; } }
    if (ctx->frame_copy) { extern void rt_gc_root_range_del(const char *); rt_gc_root_range_del((const char *)ctx->frame_copy); free(ctx->frame_copy); ctx->frame_copy = NULL; }
    sem_destroy(ctx->semp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coret(uint64_t d0, uint64_t d1, void *resume_addr) {
    if (!scrip_co_current) scrip_co_uerror("scrip_coexpr: scrip_coret with no current coexpression (RUNG 5 `@` wiring bug -- body entered without setting scrip_co_current)");
    scrip_co_current->xmit[0] = d0;
    scrip_co_current->xmit[1] = d1;
    scrip_co_current->resume_addr = resume_addr;
    scrip_coctx_t *me = scrip_co_current;
    scrip_coctx_t *back = me->activator;
    if (!back) scrip_co_uerror("scrip_coexpr: scrip_coret with no activator (RUNG 5 `@` did not set scrip_co_current->activator before switching in)");
    scrip_co_current = back;
    scrip_coswitch(me, back, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_cofail(void) {
    if (!scrip_co_current) scrip_co_uerror("scrip_coexpr: scrip_cofail with no current coexpression (RUNG 5 `@` wiring bug)");
    scrip_coctx_t *me = scrip_co_current;
    me->dead = 1;
    scrip_coctx_t *back = me->activator;
    if (!back) scrip_co_uerror("scrip_coexpr: scrip_cofail with no activator (RUNG 5 `@` did not set the activator chain)");
    scrip_co_current = back;
    scrip_coswitch(me, back, 1);
}
typedef struct scrip_coexpr_entry_pkg_t {
    void    *body_entry_addr;
    uint64_t r12, r13, r14, r15, rbx, csav5;
} scrip_coexpr_entry_pkg_t;
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, body_entry_addr) ==  0, "pkg layout drift: body_entry_addr");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r12)             ==  8, "pkg layout drift: r12");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r13)             == 16, "pkg layout drift: r13");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r14)             == 24, "pkg layout drift: r14");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r15)             == 32, "pkg layout drift: r15");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbx)             == 40, "pkg layout drift: rbx");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, csav5)             == 48, "pkg layout drift: csav5");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coexpr_trampoline_entry(void *arg) {
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)arg;
    __asm__ volatile (
        "mov  0(%0), %%rax\n\t"
        "mov  8(%0), %%r12\n\t"
        "mov 16(%0), %%r13\n\t"
        "mov 24(%0), %%r14\n\t"
        "mov 32(%0), %%r15\n\t"
        "mov 40(%0), %%rbx\n\t"
        "mov 48(%0), %%r11\n\t.byte 0x4c,0x89,0xdd\n\t"
        "jmp *%%rax\n\t"
        :
        : "r"(pkg)
        : "rax", "r12", "r13", "r14", "r15", "rbx", "memory"
    );
    fprintf(stderr, "scrip_coexpr: FATAL scrip_coexpr_trampoline_entry fell through the jmp -- bad body_entry_addr?\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
scrip_coctx_t *scrip_coexpr_create(void *body_entry_addr, const uint64_t regs[6], uint64_t frame_bytes) {
    extern long g_scrip_coexpr_live; g_scrip_coexpr_live++;
    scrip_coctx_t *ctx = (scrip_coctx_t *)malloc(sizeof(scrip_coctx_t));
    if (!ctx) scrip_co_uerror("scrip_coexpr: malloc scrip_coctx_t failed");
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)malloc(sizeof(scrip_coexpr_entry_pkg_t));
    if (!pkg) scrip_co_uerror("scrip_coexpr: malloc scrip_coexpr_entry_pkg_t failed");
    pkg->body_entry_addr = body_entry_addr;
    pkg->r12 = regs[0]; pkg->r13 = regs[1]; pkg->r14 = regs[2];
    pkg->r15 = regs[3]; pkg->rbx = regs[4]; pkg->csav5 = regs[5];
    ctx->frame_copy = NULL; ctx->frame_copy_sz = 0;
    if (frame_bytes > 0 && regs[5] != 0) {
        extern void rt_gc_root_range_add(const char *, const char *);
        void *cp = malloc((size_t)frame_bytes);
        if (!cp) scrip_co_uerror("scrip_coexpr: malloc frame snapshot failed");
        memcpy(cp, (const void *)(uintptr_t)regs[5], (size_t)frame_bytes);
        pkg->csav5 = (uint64_t)(uintptr_t)cp;
        ctx->frame_copy = cp; ctx->frame_copy_sz = frame_bytes;
        rt_gc_root_range_add((const char *)cp, (const char *)cp + frame_bytes);
    }
    ctx->entry_fn  = scrip_coexpr_trampoline_entry;
    ctx->entry_arg = pkg;
    ctx->thread = 0;
    ctx->alive = 0;
    ctx->semp  = NULL;
    ctx->activator   = NULL;
    ctx->resume_addr = NULL;
    ctx->dead        = 0;
    ctx->xmit[0]     = 0;
    ctx->xmit[1]     = 0;
    ctx->stk_lo      = 0;
    ctx->stk_hi      = 0;
    for (int i = 0; i < 6; i++) ctx->gc_spill[i] = 0;
    ctx->scan_state = NULL;
    ctx->gc_next = g_co_gc_head; g_co_gc_head = ctx;
    return ctx;
}
static scrip_coctx_t g_root_ctx;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scrip_coexpr_activate(scrip_coctx_t *target, uint64_t x0, uint64_t x1, uint64_t *out2) {
    if (!target) scrip_co_uerror("scrip_coexpr: activate of NULL coexpression (operand slot held garbage -- LOWER/driver wiring bug)");
    if (target->dead) return 0;
    scrip_coctx_t *self = scrip_co_current ? scrip_co_current : &g_root_ctx;
    scrip_coctx_t *prev = scrip_co_current;
    int first = target->alive ? 1 : 0;
    target->activator = self;
    target->xmit[0] = x0;
    target->xmit[1] = x1;
    scrip_co_current = target;
    scrip_coswitch(self, target, first);
    scrip_co_current = prev;
    if (target->dead) return 0;
    out2[0] = target->xmit[0];
    out2[1] = target->xmit[1];
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_co_ctx_init(scrip_coctx_t *ctx, void (*entry_fn)(void *), void *entry_arg) {
    extern long g_scrip_coexpr_live; g_scrip_coexpr_live++;
    ctx->entry_fn  = entry_fn;
    ctx->entry_arg = entry_arg;
    ctx->thread = 0;
    ctx->alive = 0;
    ctx->semp  = NULL;
    ctx->activator   = NULL;
    ctx->resume_addr = NULL;
    ctx->dead        = 0;
    ctx->xmit[0]     = 0;
    ctx->xmit[1]     = 0;
    ctx->stk_lo      = 0;
    ctx->stk_hi      = 0;
    for (int i = 0; i < 6; i++) ctx->gc_spill[i] = 0;
    ctx->frame_copy = NULL; ctx->frame_copy_sz = 0;
    ctx->scan_state = NULL;
    ctx->gc_next = NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_co_gc_link(scrip_coctx_t *ctx) { ctx->gc_next = g_co_gc_head; g_co_gc_head = ctx; }
scrip_coctx_t *scrip_co_gc_head(void) { return g_co_gc_head; }
scrip_coctx_t *scrip_co_gc_root(void) { return &g_root_ctx; }
int scrip_co_main_known(pthread_t *out) { if (g_co_main_set && out) *out = g_co_main_thr; return g_co_main_set; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scrip_co_stack_of(scrip_coctx_t *ctx, char **lo, char **hi) {
    if (ctx->stk_lo && ctx->stk_hi && ctx->stk_lo < ctx->stk_hi) { *lo = ctx->stk_lo; *hi = ctx->stk_hi; return 1; }   /* THE BOUNDS ARE READ BACK, NEVER RE-DERIVED -- recorded once in scrip_coswitch, right after the thread that owns them is created. */
    if (!ctx->alive || ctx->thread == 0) return 0;   /* not yet activated, or activated but no thread exists for it yet (row coexpr-stack-of-calls-pthread-getattr-np-on-an-uncreated-thread) -- never ask pthread about a thread that may not exist. */
    { pthread_attr_t a; void *sa = 0; size_t sz = 0;   /* remaining legitimate use: the MAIN/ROOT context, whose thread is pthread_self() and which records no stk_lo/stk_hi of its own. */
      if (pthread_getattr_np(ctx->thread, &a) != 0) return 0;
      if (pthread_attr_getstack(&a, &sa, &sz) != 0) { pthread_attr_destroy(&a); return 0; }
      pthread_attr_destroy(&a); *lo = (char *)sa; *hi = (char *)sa + sz; return 1; }
}
