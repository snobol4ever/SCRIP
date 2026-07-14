/* rt_gram_trampoline.S - RK-GRAM-3b scan-entry trampoline (runtime plumbing; emits no program code).        */
/* DESCR_t rk_gram_enter_box(bb_box_fn fn, const char* Sigma, long Delta, void* zeta, long* out_delta)        */
/* Loads Sigma/delta/Delta into the callee-saved subject triad r13/r14/r15, enters the grammar box at its     */
/* alpha port (entry=0 in esi, zeta frame in rdi -> the box does push r12; mov r12,rdi), then reads the       */
/* advanced cursor delta back out of r14 into *out_delta. The box clobbers r13/r14/r15/rbx without restoring  */
/* them (stackless-box discipline: the entering caller saves them), so this shim preserves all four. r8       */
/* (out_delta, caller-saved) is stacked across the box call because the box's strchr/memcmp may clobber it;   */
/* the five 8-byte pushes also leave rsp 16-aligned at the call (SysV) so the box's internal SSE-spilling      */
/* libc calls stay aligned. The box returns spec_t in rax:rdx (v=1 match / v=99 DT_FAIL); it flows straight   */
/* back through this shim's epilogue, which touches only rcx and the saved registers, never rax/rdx.          */
    .text
    .globl rk_gram_enter_box
    .type  rk_gram_enter_box, @function
rk_gram_enter_box:
    pushq %r13
    pushq %r14
    pushq %r15
    pushq %rbx
    pushq %r8
    movq  %rsi, %r13
    xorl  %r14d, %r14d
    movq  %rdx, %r15
    movq  %rdi, %rax
    movq  %rcx, %rdi
    xorl  %esi, %esi
    call  *%rax
    popq  %rcx
    movq  %r14, (%rcx)
    popq  %rbx
    popq  %r15
    popq  %r14
    popq  %r13
    ret
    .size  rk_gram_enter_box, .-rk_gram_enter_box
    .section .note.GNU-stack,"",@progbits
