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
    .size  rt_deref, .-rt_deref
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl to_int
    .type  to_int, @function
to_int:
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
    .size  to_int, .-to_int
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_gc_point_arr
    .type  rt_gc_point_arr, @function
rt_gc_point_arr:
    pushq  %rbx
    pushq  %rbp
    pushq  %r12
    pushq  %r13
    pushq  %r14
    pushq  %r15
    subq   $8, %rsp
    leaq   8(%rsp), %rcx
    call   rt_gc_point_arr_c@PLT
    addq   $8, %rsp
    popq   %r15
    popq   %r14
    popq   %r13
    popq   %r12
    popq   %rbp
    popq   %rbx
    ret
    .size  rt_gc_point_arr, .-rt_gc_point_arr
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_gc_collect
    .type  rt_gc_collect, @function
rt_gc_collect:
    pushq  %rbx
    pushq  %rbp
    pushq  %r12
    pushq  %r13
    pushq  %r14
    pushq  %r15
    subq   $8, %rsp
    leaq   8(%rsp), %rdi
    call   rt_gc_collect_c@PLT
    addq   $8, %rsp
    popq   %r15
    popq   %r14
    popq   %r13
    popq   %r12
    popq   %rbp
    popq   %rbx
    ret
    .size  rt_gc_collect, .-rt_gc_collect
    .section .note.GNU-stack,"",@progbits
