/* rt_gram_trampoline.S - RK-GRAM-3b scan-entry trampoline (runtime plumbing; emits no program code).        */
#include "descr_tags.inc"
/* DESCR_t rk_gram_enter_box(bb_box_fn fn, const char* Sigma, long Delta, void* zeta, long* out_delta)       */
/* SysV x86-64 args: rdi=fn, rsi=Sigma, rdx=Delta, rcx=zeta, r8=out_delta.                                  */
/*                                                                                                            */
/* RK-ZC-3: updated for zframe_graph=1 boxes (ICN-FR-2 wire-header prologue).                                */
/* Old protocol (pre-RK-ZC-2): box entered via 'call'; rdi=zeta, rsi=0 on entry.                            */
/* New protocol: box prologue is                                                                               */
/*   sub rsp,64; mov [rsp+40],rcx (γ wire); mov [rsp+48],rdx (ω wire);                                      */
/*   mov [rsp+56],___; mov ___,rsp                                                                            */
/* exits via 'jmp rcx' (γ) or 'jmp rdx' (ω). We supply rcx=γ-label, rdx=ω-label before jmp.                */
/* Use 'jmp *rax' NOT 'call': the zframe kt is sized for jmp entry (icn_zf_main_call law).                   */
/*                                                                                                            */
/* DESCR_t = 16 bytes. SysV struct-return in rax:rdx:                                                        */
/*   rax = bytes 0-7:  DTYPE_t v (4B) | uint32_t slen (4B)                                                   */
/*   rdx = bytes 8-15: union { int64_t i; ... }                                                               */
/* FAILDESCR: rax=0x0000000000000068 (DT_FAIL=0x68), rdx=0.                                                  */
/* γ result DESCR_t is already in rax:rdx from the box; forward unchanged.                                   */
/*                                                                                                            */
/* Stack layout (rsp 16-aligned on function entry after -8 for ret-addr):                                    */
/*   push r13/r14/r15/rbx/rcx = 5*8=40 bytes, then sub rsp,8 = 48 total.                                    */
/*   Box sub rsp,64 → 112 bytes total; 112 mod 16 = 0 (aligned for SSE/libc calls inside box).              */
    .text
    .globl rk_gram_enter_box
    .type  rk_gram_enter_box, @function
rk_gram_enter_box:
    pushq %r13
    pushq %r14
    pushq %r15
    pushq %rbx
    pushq %rcx                   /* save zeta (4th arg); will reload into rdi before jmp */
    subq  $8, %rsp               /* 16-align: 5*8 pushes + 8 ret-addr = 48 below 16-boundary; pad to 48 */
    movq  %r8, %rbx              /* stash out_delta in rbx (callee-saved across box) */
    movq  %rdi, %rax             /* rax = fn (move first, before rdi is clobbered) */
    movq  %rsi, %r13             /* r13 = Sigma */
    xorl  %r14d, %r14d           /* r14 = delta = 0 */
    movq  %rdx, %r15             /* r15 = Delta */
    movq  8(%rsp), %rdi          /* rdi = zeta (reloaded from stack: pushed as rcx 2nd-to-last) */
    xorl  %esi, %esi             /* rsi = 0 (alpha entry) */
    leaq  .Lgrambox_γ(%rip), %rcx  /* γ wire: box prologue saves [rsp+40]=rcx */
    leaq  .Lgrambox_ω(%rip), %rdx  /* ω wire: box prologue saves [rsp+48]=rdx */
    jmp   *%rax                  /* RK-ZC-3: jmp not call (zframe kt sized for jmp) */
.Lgrambox_γ:
    /* γ: match. rax:rdx = result DESCR_t from box. r14 = final delta. */
    movq  %r14, (%rbx)           /* *out_delta = final delta */
    addq  $8, %rsp               /* undo alignment pad */
    popq  %rcx                   /* discard saved zeta */
    popq  %rbx
    popq  %r15
    popq  %r14
    popq  %r13
    ret
.Lgrambox_ω:
    /* ω: fail. Return FAILDESCR: rax=0x68 (DT_FAIL), rdx=0. */
    movl  $DT_FAIL, %eax         /* single source descr_tags.inc; movl zeros upper 32b of rax */
    xorl  %edx, %edx
    movq  %r14, (%rbx)           /* *out_delta = delta (unchanged on full fail = 0) */
    addq  $8, %rsp
    popq  %rcx
    popq  %rbx
    popq  %r15
    popq  %r14
    popq  %r13
    ret
    .size  rk_gram_enter_box, .-rk_gram_enter_box
    .section .note.GNU-stack,"",@progbits
