#define _GNU_SOURCE
#include <errno.h>
#include "ct_arena.h"
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
#include "gc_heap.h"
#include "rt_coexpr.h"
static int inited = 0;
static pthread_attr_t attribs;
static long g_coexp_stksize = 8L * 1024 * 1024;
scrip_coctx_t *scrip_co_current = NULL;
static scrip_coctx_t *g_co_gc_head = NULL;
static pthread_t g_co_main_thr;
static int g_co_main_set = 0;
static long g_coexpr_serial = 1;
extern int scan_depth;
typedef struct scrip_coexpr_entry_pkg_t {
    void    *body_entry_addr;
    uint64_t r12, r13, r14, r15, rbx, csav5, gva, frame_bytes, below;
} scrip_coexpr_entry_pkg_t;
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, body_entry_addr) ==  0, "pkg layout drift: body_entry_addr");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r12)             ==  8, "pkg layout drift: r12");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r13)             == 16, "pkg layout drift: r13");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r14)             == 24, "pkg layout drift: r14");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, r15)             == 32, "pkg layout drift: r15");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, rbx)             == 40, "pkg layout drift: rbx");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, csav5)             == 48, "pkg layout drift: csav5");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, gva)               == 56, "pkg layout drift: gva -- the GLOBAL-VARIABLE AREA base, r9. A co-expression body reads every global as [r9 + off], and r9 was not among the six registers this package carried, so on the body's own thread it held whatever the trampoline left there: EVERY GLOBAL READ INSIDE A CO-EXPRESSION RETURNED GARBAGE, which is also why activating a co-expression stored in a global segfaulted -- the target pointer was garbage, not the co-expression.");
_Static_assert(offsetof(scrip_coexpr_entry_pkg_t, frame_bytes)       == 64, "pkg layout drift: frame_bytes -- the trampoline reads it at 64(pkg) to size the frame-snapshot restore onto the body stack");
_Static_assert(sizeof(DESCR_t) == 16, "the transmitted value is one 16-byte DESCR: bb_activate stages it as rsi:rdx and bb_coret as rdi:rsi, and scrip_coret/scrip_coexpr_activate write exactly those two words");
static void co_xmit_set(DESCR_t *x, uint64_t d0, uint64_t d1) { uint64_t w[2]; w[0] = d0; w[1] = d1; memcpy(x, w, sizeof *x); }
int scrip_co_gc_plant(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_GC_COEXPR_PLANT"); v = (e && *e) ? atoi(e) : 0; } return v; }
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
    if (self->eager) {
        if (self->image_span) { char *img = (char *)__builtin_alloca(self->image_span + 16); img = (char *)(((uintptr_t)img + 15u) & ~(uintptr_t)15u);
            memcpy(img, self->image_src, self->image_span); self->image = img;
            { scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)self->entry_arg; pkg->csav5 = (uint64_t)(uintptr_t)img; pkg->frame_bytes = (uint64_t)self->image_span; pkg->below = self->image_below; } }
        sem_post(&self->created);
    }
    while (sem_wait(self->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in trampoline");
    scrip_co_current = self;
    if (setjmp(self->exit_jmp) != 0) return NULL;
    self->entry_fn(self->entry_arg);
    fprintf(stderr, "scrip_coexpr: FATAL entry_fn returned to trampoline instead of switching away -- RUNG 3/4 bug\n");
    abort();
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scrip_co_init_once(scrip_coctx_t *cur) {
    if (inited) return;
    cur->semp = &cur->sema;
    scrip_co_makesem(cur);
    cur->thread = pthread_self();
    cur->alive = 1;
    g_co_main_thr = cur->thread; g_co_main_set = 1;
    { const char *_cs = getenv("SCRIP_COEXP_STACK"); if (_cs && *_cs) { long _v = atol(_cs); if (_v >= (long)PTHREAD_STACK_MIN) g_coexp_stksize = _v; } }
    pthread_attr_init(&attribs);
    if (pthread_attr_setstacksize(&attribs, (size_t)g_coexp_stksize) != 0)
        scrip_co_uerror("scrip_coexpr: pthread_attr_setstacksize failed");
    inited = 1;
}
static void scrip_co_thread_start(scrip_coctx_t *new_ctx) {
    scrip_co_makesem(new_ctx);
    { pthread_attr_t *ap = &attribs; pthread_attr_t big;
      size_t need = new_ctx->stk_need + (size_t)(2u << 20);
      if (need > (size_t)g_coexp_stksize) { need = (need + 4095u) & ~(size_t)4095u; pthread_attr_init(&big);
          if (pthread_attr_setstacksize(&big, need) != 0) scrip_co_uerror("scrip_coexpr: pthread_attr_setstacksize (image-sized) failed"); ap = &big; }
      if (pthread_create(&new_ctx->thread, ap, scrip_co_trampoline, new_ctx) != 0)
          scrip_co_uerror("scrip_coexpr: pthread_create failed");
      if (ap == &big) pthread_attr_destroy(&big); }
    { pthread_attr_t a; void *sa = 0; size_t sz = 0;
      if (pthread_getattr_np(new_ctx->thread, &a) != 0) scrip_co_uerror("scrip_coexpr: pthread_getattr_np on new thread failed");
      if (pthread_attr_getstack(&a, &sa, &sz) != 0) scrip_co_uerror("scrip_coexpr: pthread_attr_getstack on new thread failed");
      pthread_attr_destroy(&a);
      new_ctx->stk_lo = (char *)sa; new_ctx->stk_hi = (char *)sa + sz; }
    new_ctx->alive = 1;
}
void scrip_coswitch(scrip_coctx_t *old, scrip_coctx_t *new_ctx, int first) {
    scrip_co_init_once(old);
    const int _inh_ctx = new_ctx && new_ctx->inherit_scan;
    { extern void *rt_scan_state_capture(void *); old->scan_state = rt_scan_state_capture(old->scan_state); }
    if (first == 0) scrip_co_thread_start(new_ctx);
    if (!new_ctx->started) { new_ctx->started = 1; { extern void rt_scan_state_reset(void); if (!_inh_ctx) rt_scan_state_reset(); } }
    __asm__ volatile ("mov %%rsp, %0" : "=m"(old->park_sp));
    { extern void rtcc_coexpr_save(uint64_t *); rtcc_coexpr_save(old->rtcc_spill); }
    sem_post(new_ctx->semp);
    while (sem_wait(old->semp) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait in scrip_coswitch");
    if (!old->alive) longjmp(old->exit_jmp, 1);
    { extern void rtcc_coexpr_restore(const uint64_t *); rtcc_coexpr_restore(old->rtcc_spill); }
    { extern void rt_scan_state_apply(void *); if (!_inh_ctx) rt_scan_state_apply(old->scan_state); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coexpr_destroy(scrip_coctx_t *ctx) {
    ctx->alive = 0;
    sem_post(ctx->semp);
    pthread_join(ctx->thread, NULL);
    ctx->stk_lo = 0; ctx->stk_hi = 0; ctx->park_sp = 0;
    { extern long g_scrip_coexpr_live; scrip_coctx_t **pp = &g_co_gc_head; while (*pp && *pp != ctx) pp = &(*pp)->gc_next; if (*pp) { *pp = ctx->gc_next; g_scrip_coexpr_live--; } }
    ctx->image = 0; ctx->image_span = 0;
    if (ctx->eager) sem_destroy(&ctx->created);
    sem_destroy(ctx->semp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static scrip_coctx_t g_root_ctx;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long scrip_co_serial_disp(scrip_coctx_t *c) { if (!c) return 0; if (c == &g_root_ctx && c->serial == 0) return 1; return c->serial; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scrip_co_trace_xmit(const char *procname, scrip_coctx_t *self, scrip_coctx_t *target, uint64_t x0, uint64_t x1) {
    extern void rt_icn_trace_coexpr(const char *procname, long self_serial, long targ_serial, uint64_t x0, uint64_t x1, int kind, long line_override);
    if (!procname || !target) return;
    long ts = scrip_co_serial_disp(target);
    if (ts == 0) return;
    rt_icn_trace_coexpr(procname, scrip_co_serial_disp(self), ts, x0, x1, 0, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scrip_co_trace_term(scrip_coctx_t *me, scrip_coctx_t *back, uint64_t d0, uint64_t d1, int failed) {
    extern void rt_icn_trace_coexpr(const char *procname, long self_serial, long targ_serial, uint64_t x0, uint64_t x1, int kind, long line_override);
    if (!me || !back || me->serial == 0 || me->create_line <= 0) return;
    rt_icn_trace_coexpr(me->create_proc ? me->create_proc : "main", me->serial, scrip_co_serial_disp(back), d0, d1, failed ? 1 : 2, me->create_line);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static scrip_coctx_t *scrip_co_live_activator(scrip_coctx_t *me) {
    scrip_coctx_t *b = me ? me->activator : 0;
    for (int hop = 0; b && b != &g_root_ctx && b->dead && hop < 64; hop++) b = b->activator;
    if (!b || (b != &g_root_ctx && b->dead)) b = &g_root_ctx;
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coret(uint64_t d0, uint64_t d1, void *resume_addr) {
    if (!scrip_co_current) scrip_co_uerror("scrip_coexpr: scrip_coret with no current coexpression (RUNG 5 `@` wiring bug -- body entered without setting scrip_co_current)");
    scrip_co_current->resume_addr = resume_addr;
    scrip_coctx_t *me = scrip_co_current;
    scrip_coctx_t *back = scrip_co_live_activator(me);
    if (!back) scrip_co_uerror("scrip_coexpr: scrip_coret with no activator (RUNG 5 `@` did not set scrip_co_current->activator before switching in)");
    co_xmit_set(&back->xmit, d0, d1);
    me->activations++;
    scrip_co_trace_term(me, back, d0, d1, 0);
    scrip_co_current = back;
    scrip_coswitch(me, back, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_cofail(void) {
    if (!scrip_co_current) scrip_co_uerror("scrip_coexpr: scrip_cofail with no current coexpression (RUNG 5 `@` wiring bug)");
    scrip_coctx_t *me = scrip_co_current;
    me->dead = 1;
    scrip_coctx_t *back = scrip_co_live_activator(me);
    if (!back) scrip_co_uerror("scrip_coexpr: scrip_cofail with no activator (RUNG 5 `@` did not set the activator chain)");
    scrip_co_trace_term(me, back, 0, 0, 1);
    scrip_co_current = back;
    scrip_coswitch(me, back, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_coexpr_trampoline_entry(void *arg) {
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)arg;
    __asm__ volatile (
        "mov  8(%0), %%r12\n\t"
        "mov 16(%0), %%r13\n\t"
        "mov 24(%0), %%r14\n\t"
        "mov 32(%0), %%r15\n\t"
        "mov 40(%0), %%rbx\n\t"
        "mov 56(%0), %%r9\n\t"
        "mov 48(%0), %%rsi\n\t"
        "mov 64(%0), %%rcx\n\t"
        "mov  0(%0), %%rax\n\t"
        "test %%rcx, %%rcx\n\t"
        "jz 2f\n\t"
        "sub %%rcx, %%rsp\n\t"
        "sub $256, %%rsp\n\t"
        "and $-16, %%rsp\n\t"
        "mov %%rsp, %%rdi\n\t"
        "mov %%rsp, %%rbp\n\t"
        "mov 72(%0), %%rdx\n\t"
        "add %%rdx, %%rbp\n\t"
        "cld\n\t"
        "rep movsb\n\t"
        "jmp *%%rax\n\t"
        "2:\n\t"
        "mov %%rsp, %%rbp\n\t"
        "jmp *%%rax\n\t"
        :
        : "r"(pkg)
        : "rax", "rcx", "rdx", "rsi", "rdi", "r12", "r13", "r14", "r15", "rbx", "r9", "memory"
    );
    fprintf(stderr, "scrip_coexpr: FATAL scrip_coexpr_trampoline_entry fell through the jmp -- bad body_entry_addr?\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
scrip_coctx_t *scrip_coexpr_create(void *body_entry_addr, const uint64_t regs[7], uint64_t frame_bytes, uint64_t below_bytes, const char *procname) {
    extern long g_scrip_coexpr_live; g_scrip_coexpr_live++;
    scrip_coctx_t *ctx = (scrip_coctx_t *)ct_alloc(sizeof(scrip_coctx_t));
    if (!ctx) scrip_co_uerror("scrip_coexpr: malloc scrip_coctx_t failed");
    scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)ct_alloc(sizeof(scrip_coexpr_entry_pkg_t));
    if (!pkg) scrip_co_uerror("scrip_coexpr: malloc scrip_coexpr_entry_pkg_t failed");
    pkg->body_entry_addr = body_entry_addr;
    ctx->inherit_scan = 0;
    pkg->r12 = regs[0]; pkg->r13 = regs[1]; pkg->r14 = regs[2];
    pkg->r15 = regs[3]; pkg->rbx = regs[4]; pkg->csav5 = regs[5]; pkg->gva = regs[6]; pkg->frame_bytes = frame_bytes; pkg->below = 0;
    ctx->image = 0; ctx->image_src = 0; ctx->image_span = 0; ctx->image_below = below_bytes; ctx->started = 0; ctx->eager = 1; ctx->stk_need = (size_t)frame_bytes;
    if (frame_bytes + below_bytes > 0 && regs[5] != 0) {
        ctx->image_span = (size_t)(frame_bytes + below_bytes);
        ctx->image_src = (const char *)(uintptr_t)(regs[5] - below_bytes);
        ctx->stk_need = 2 * ctx->image_span;
    }
    ctx->entry_fn  = scrip_coexpr_trampoline_entry;
    ctx->entry_arg = pkg;
    ctx->thread = 0;
    ctx->alive = 0;
    ctx->semp  = NULL;
    ctx->activator   = NULL;
    ctx->resume_addr = NULL;
    ctx->dead        = 0;
    memset(&ctx->xmit, 0, sizeof ctx->xmit);
    ctx->stk_lo      = 0;
    ctx->stk_hi      = 0;
    ctx->park_sp     = 0;
    ctx->sigma_live  = scan_depth > 0;
    ctx->scan_state = NULL;
    ctx->serial = ++g_coexpr_serial;
    ctx->activations = 0;
    { extern long g_line; ctx->create_line = g_line; }
    ctx->create_proc = procname;
    ctx->cur_line = 0;
    ctx->gc_next = g_co_gc_head; g_co_gc_head = ctx;
    { scrip_coctx_t *cur = scrip_co_current ? scrip_co_current : &g_root_ctx; scrip_co_init_once(cur); }
    if (sem_init(&ctx->created, 0, 0) == -1) scrip_co_uerror("scrip_coexpr: sem_init (created) failed");
    scrip_co_thread_start(ctx);
    while (sem_wait(&ctx->created) < 0) if (errno != EINTR) scrip_co_uerror("scrip_coexpr: sem_wait (created) in scrip_coexpr_create");
    return ctx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
scrip_coctx_t *scrip_coexpr_refresh(scrip_coctx_t *orig) {
    if (!orig) scrip_co_uerror("scrip_coexpr: refresh of NULL coexpression");
    scrip_coexpr_entry_pkg_t *opkg = (scrip_coexpr_entry_pkg_t *)orig->entry_arg;
    if (!opkg) scrip_co_uerror("scrip_coexpr: refresh of a coexpression with no entry package");
    uint64_t regs[7];
    regs[0] = opkg->r12; regs[1] = opkg->r13; regs[2] = opkg->r14; regs[3] = opkg->r15;
    regs[4] = opkg->rbx; regs[5] = opkg->csav5 + opkg->below; regs[6] = opkg->gva;
    return scrip_coexpr_create(opkg->body_entry_addr, regs, orig->image_span ? (uint64_t)orig->image_span - orig->image_below : 0, orig->image_below, orig->create_proc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scrip_coexpr_activate(scrip_coctx_t *target, uint64_t x0, uint64_t x1, uint64_t *out2, const char *procname) {
    if (!target) scrip_co_uerror("scrip_coexpr: activate of NULL coexpression (operand slot held garbage -- LOWER/driver wiring bug)");
    scrip_coctx_t *self = scrip_co_current ? scrip_co_current : &g_root_ctx;
    if (target->dead) { scrip_co_trace_xmit(procname, self, target, x0, x1); scrip_co_trace_term(target, self, 0, 0, 1); return 0; }
    scrip_coctx_t *prev = scrip_co_current;
    int first = target->alive ? 1 : 0;
    scrip_co_trace_xmit(procname, self, target, x0, x1);
    target->activator = self;
    co_xmit_set(&target->xmit, x0, x1);
    { extern long g_line; self->cur_line = g_line; if (!target->alive && target->create_line > 0) g_line = target->create_line; }
    scrip_co_current = target;
    scrip_coswitch(self, target, first);
    scrip_co_current = prev;
    { extern long g_line; if (self->cur_line > 0) g_line = self->cur_line; }
    if (target->dead) return 0;
    memcpy(out2, &self->xmit, sizeof self->xmit);
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
    ctx->inherit_scan = 0;
    ctx->resume_addr = NULL;
    ctx->dead        = 0;
    memset(&ctx->xmit, 0, sizeof ctx->xmit);
    ctx->stk_lo      = 0;
    ctx->stk_hi      = 0;
    ctx->park_sp     = 0;
    ctx->sigma_live  = scan_depth > 0;
    ctx->image = 0; ctx->image_src = 0; ctx->image_span = 0; ctx->image_below = 0; ctx->started = 0; ctx->eager = 0;
    ctx->scan_state = NULL;
    ctx->serial = 0;
    ctx->activations = 0;
    ctx->create_line = 0;
    ctx->create_proc = NULL;
    ctx->cur_line = 0;
    ctx->gc_next = NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scrip_co_gc_link(scrip_coctx_t *ctx) { ctx->gc_next = g_co_gc_head; g_co_gc_head = ctx; }
void rt_coexpr_gc_scan_states(void)
{
    extern void gen_gc_visit_scan_state(void *);
    scrip_coctx_t *c;
    gen_gc_visit_scan_state(g_root_ctx.scan_state);
    for (c = g_co_gc_head; c; c = c->gc_next) gen_gc_visit_scan_state(c->scan_state);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coexpr_gc_audit_scan_states(long *hp, long *un)
{
    extern void gen_gc_audit_scan_state(void *, long *, long *);
    scrip_coctx_t *c;
    gen_gc_audit_scan_state(g_root_ctx.scan_state, hp, un);
    for (c = g_co_gc_head; c; c = c->gc_next) gen_gc_audit_scan_state(c->scan_state, hp, un);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
scrip_coctx_t *scrip_co_gc_head(void) { return g_co_gc_head; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void rt_genp_thread_entry(void *arg);
static void co_gc_visit_record(scrip_coctx_t *c, long *n_sigma)
{
    if (!c->entry_arg || !c->sigma_live) return;
    if (c->entry_fn == scrip_coexpr_trampoline_entry) { scrip_coexpr_entry_pkg_t *pkg = (scrip_coexpr_entry_pkg_t *)c->entry_arg; rt_gc_visit_raw((const char **)&pkg->r13); (*n_sigma)++; return; }
    if (c->entry_fn == rt_genp_thread_entry) { rt_gc_visit_raw((const char **)((char *)c->entry_arg + 24)); (*n_sigma)++; return; }
}
void scrip_co_gc_images(long *on_stack, long *off_stack)
{
    long on = 0, off = 0; scrip_coctx_t *c;
    for (c = g_co_gc_head; c; c = c->gc_next) if (c->image) { if (c->stk_lo && c->image >= c->stk_lo && c->image < c->stk_hi) on++; else off++; }
    if (on_stack) *on_stack = on;
    if (off_stack) *off_stack = off;
}
long scrip_co_gc_visit_records(long *n_ctx, long *n_sigma)
{
    long n = 0, ns = 0; scrip_coctx_t *c;
    if (scrip_co_gc_plant() == 1) { if (n_ctx) *n_ctx = 0; if (n_sigma) *n_sigma = 0; return 0; }
    rt_gc_visit_descr(&g_root_ctx.xmit); n++;
    for (c = g_co_gc_head; c; c = c->gc_next) { rt_gc_visit_descr(&c->xmit); co_gc_visit_record(c, &ns); n++; }
    if (n_ctx) *n_ctx = n;
    if (n_sigma) *n_sigma = ns;
    return n;
}
scrip_coctx_t *scrip_co_gc_root(void) { if (g_root_ctx.serial == 0) g_root_ctx.serial = 1; return &g_root_ctx; }
long scrip_coexpr_serial_of(void *ctx) { return ctx ? ((scrip_coctx_t *)ctx)->serial : 0; }
long scrip_coexpr_activations_of(void *ctx) { if (!ctx) return 0; if (ctx == (void *)&g_root_ctx) return ((scrip_coctx_t *)ctx)->activations + 1; return ((scrip_coctx_t *)ctx)->activations; }
int scrip_co_main_known(pthread_t *out) { if (g_co_main_set && out) *out = g_co_main_thr; return g_co_main_set; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scrip_co_stack_of(scrip_coctx_t *ctx, char **lo, char **hi) {
    if (ctx->stk_lo && ctx->stk_hi && ctx->stk_lo < ctx->stk_hi) { *lo = ctx->stk_lo; *hi = ctx->stk_hi; return 1; }
    if (!ctx->alive || ctx->thread == 0) return 0;
    { pthread_attr_t a; void *sa = 0; size_t sz = 0;
      if (pthread_getattr_np(ctx->thread, &a) != 0) return 0;
      if (pthread_attr_getstack(&a, &sa, &sz) != 0) { pthread_attr_destroy(&a); return 0; }
      pthread_attr_destroy(&a); *lo = (char *)sa; *hi = (char *)sa + sz; return 1; }
}
