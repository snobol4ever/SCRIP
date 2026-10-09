#include "descr_tags.inc"
    .data
    .align 1
    .globl rtx_gate_leaf
    .hidden rtx_gate_leaf
    .type  rtx_gate_leaf, @object
    .size  rtx_gate_leaf, 1
rtx_gate_leaf:
    .byte  0
    .text
    .globl rt_deref
    .type  rt_deref, @function
rt_deref:
    .cfi_startproc
    cmpb   $0, rtx_gate_leaf(%rip)
    jne    .Lrd_asm
    jmp    rt_deref_slow@PLT
.Lrd_asm:
    cmpb   $DT_N, %dil
    jne    .Lrd_retd
    movq   %rdi, %rax
    shrq   $32, %rax
    cmpl   $1, %eax
    je     .Lrd_ptr
    cmpl   $2, %eax
    je     .Lrd_ntrap
    testl  %eax, %eax
    je     .Lrd_name0
.Lrd_retd:
    movq   %rdi, %rax
    movq   %rsi, %rdx
    ret
.Lrd_ptr:
    testq  %rsi, %rsi
    jz     .Lrd_retd
    movq   8(%rsi), %rdx
    movq   (%rsi), %rax
    ret
.Lrd_name0:
    testq  %rsi, %rsi
    jz     .Lrd_retd
    cmpb   $0, (%rsi)
    je     .Lrd_retd
    movq   %rsi, %rdi
    jmp    NV_GET_fn@PLT
.Lrd_ntrap:
    testq  %rsi, %rsi
    jz     .Lrd_fail
    movq   (%rsi), %rax
    testq  %rax, %rax
    jz     .Lrd_slow
    movq   8(%rax), %rdx
    movq   (%rax), %rax
    ret
.Lrd_fail:
    movl   $DT_FAIL, %eax
    xorl   %edx, %edx
    ret
.Lrd_slow:
    jmp    rt_deref_slow@PLT
    .cfi_endproc
    .size  rt_deref, .-rt_deref
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl to_int
    .type  to_int, @function
to_int:
    .cfi_startproc
    cmpb   $0, rtx_gate_leaf(%rip)
    jne    .Lti_asm
    jmp    to_int_slow@PLT
.Lti_asm:
    cmpb   $DT_I, %dil
    jne    .Lti_slow
    movq   %rsi, %rax
    ret
.Lti_slow:
    jmp    to_int_slow@PLT
    .cfi_endproc
    .size  to_int, .-to_int
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_gc_point_arr
    .type  rt_gc_point_arr, @function
rt_gc_point_arr:
    .cfi_startproc
    pushq  %rbx ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %rbx, 0
    pushq  %rbp ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %rbp, 0
    pushq  %r12 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r12, 0
    pushq  %r13 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r13, 0
    pushq  %r14 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r14, 0
    pushq  %r15 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r15, 0
    subq   $8, %rsp ; .cfi_adjust_cfa_offset 8
    leaq   8(%rsp), %rcx
    testq  %rdx, %rdx
    jnz    .Lgpa_r0
    leaq   24(%rsp), %rdx
    call   rt_gc_point_arr_probe_c@PLT
    jmp    .Lgpa_done
.Lgpa_r0:
    call   rt_gc_point_arr_c@PLT
.Lgpa_done:
    addq   $8, %rsp ; .cfi_adjust_cfa_offset -8
    popq   %r15 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r15
    popq   %r14 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r14
    popq   %r13 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r13
    popq   %r12 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r12
    popq   %rbp ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %rbp
    popq   %rbx ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %rbx
    ret
    .cfi_endproc
    .size  rt_gc_point_arr, .-rt_gc_point_arr
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_gc_collect
    .type  rt_gc_collect, @function
rt_gc_collect:
    .cfi_startproc
    pushq  %rbx ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %rbx, 0
    pushq  %rbp ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %rbp, 0
    pushq  %r12 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r12, 0
    pushq  %r13 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r13, 0
    pushq  %r14 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r14, 0
    pushq  %r15 ; .cfi_adjust_cfa_offset 8 ; .cfi_rel_offset %r15, 0
    subq   $8, %rsp ; .cfi_adjust_cfa_offset 8
    leaq   8(%rsp), %rdi
    call   rt_gc_collect_c@PLT
    addq   $8, %rsp ; .cfi_adjust_cfa_offset -8
    popq   %r15 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r15
    popq   %r14 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r14
    popq   %r13 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r13
    popq   %r12 ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %r12
    popq   %rbp ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %rbp
    popq   %rbx ; .cfi_adjust_cfa_offset -8 ; .cfi_restore %rbx
    ret
    .cfi_endproc
    .size  rt_gc_collect, .-rt_gc_collect
    .section .note.GNU-stack,"",@progbits
    .text
    .globl rt_tiny_glue_enter
    .type  rt_tiny_glue_enter, @function
rt_tiny_glue_enter:
    movq   %rdx, %rsi
    shrq   $8, %rsi
    movl   %esi, %esi
    leaq   39(,%rsi,8), %rcx
    andq   $-16, %rcx
    movq   %rsi, %rdx
    shlq   $4, %rdx
    addq   %rcx, %rdx
    subq   %rdx, %rsp
    movq   %rsi, 0(%rsp)
    leaq   .Ltge_gamma(%rip), %rbx
    movq   %rbx, 8(%rsp)
    leaq   .Ltge_omega(%rip), %rbx
    movq   %rbx, 16(%rsp)
    movq   g_call_args@GOTPCREL(%rip), %rdi
    movq   (%rdi), %rdi
    xorl   %edx, %edx
    testq  %rsi, %rsi
    jz     .Ltge_go
.Ltge_copy:
    movq   %rcx, 24(%rsp,%rdx,8)
    movq   0(%rdi), %rbx
    movq   %rbx, 0(%rsp,%rcx,1)
    movq   8(%rdi), %rbx
    movq   %rbx, 8(%rsp,%rcx,1)
    addq   $16, %rdi
    addq   $16, %rcx
    addq   $1, %rdx
    cmpq   %rsi, %rdx
    jb     .Ltge_copy
.Ltge_go:
    movq   %rsp, %rcx
    jmp    *%rax
    .globl rt_tge_gamma_ret
rt_tge_gamma_ret:
.Ltge_gamma:
    movq   0(%rsp), %rcx
    leaq   39(,%rcx,8), %rbx
    andq   $-16, %rbx
    shlq   $4, %rcx
    addq   %rcx, %rbx
    addq   %rbx, %rsp
    jmp    *8(%rsp)
    .globl rt_tge_omega_ret
rt_tge_omega_ret:
.Ltge_omega:
    movq   0(%rsp), %rcx
    leaq   39(,%rcx,8), %rbx
    andq   $-16, %rbx
    shlq   $4, %rcx
    addq   %rcx, %rbx
    addq   %rbx, %rsp
    jmp    *16(%rsp)
    .size  rt_tiny_glue_enter, .-rt_tiny_glue_enter
