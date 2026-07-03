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
static int inited = 0;
static pthread_attr_t attribs;
static long g_coexp_stksize = 1024 * 1024;
scrip_coctx_t *scrip_co_current = NULL;
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
        pthread_attr_init(&attribs);
        if (pthread_attr_setstacksize(&attribs, (size_t)g_coexp_stksize) != 0)
            scrip_co_uerror("scrip_coexpr: cannot set coexpression stack size");
        inited = 1;
    }
    if (first != 0) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct scrip_coexpr_entry_pkg_t {
    void    *body_entry_addr;
    uint64_t r12, r13, r14, r15, rbx, rbp;
} scrip_coexpr_entry_pkg_t;
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, body_entry_addr) ==  0, "pkg layout drift: body_entry_addr");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r12)             ==  8, "pkg layout drift: r12");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r13)             == 16, "pkg layout drift: r13");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r14)             == 24, "pkg layout drift: r14");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r15)             == 32, "pkg layout drift: r15");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbx)             == 40, "pkg layout drift: rbx");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbp)             == 48, "pkg layout drift: rbp");
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
        "mov 48(%0), %%rbp\n\t"
        "jmp *%%rax\n\t"
        :
        : "r"(pkg)
        : "rax", "r12", "r13", "r14", "r15", "rbx", "memory"
    );
    fprintf(stderr, "scrip_coexpr: FATAL scrip_coexpr_trampoline_entry fell through the jmp -- bad body_entry_addr?\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    ctx->alive = 0;
    ctx->semp  = NULL;
    ctx->activator   = NULL;
    ctx->resume_addr = NULL;
    ctx->dead        = 0;
    ctx->xmit[0]     = 0;
    ctx->xmit[1]     = 0;
    return ctx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static scrip_coctx_t g_root_ctx;
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
