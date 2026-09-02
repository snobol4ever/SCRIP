#include "descr_tags.inc"
    .text
    .globl rk_gram_enter_box
    .type  rk_gram_enter_box, @function
rk_gram_enter_box:
    pushq %r13
    pushq %r14
    pushq %r15
    pushq %rbx
    pushq %rcx
    subq  $8, %rsp
    movq  %r8, %rbx
    movq  %rdi, %rax
    movq  %rsi, %r13
    xorl  %r14d, %r14d
    movq  %rdx, %r15
    movq  8(%rsp), %rdi
    xorl  %esi, %esi
    leaq  .Lgrambox_γ(%rip), %rcx
    leaq  .Lgrambox_ω(%rip), %rdx
    jmp   *%rax
.Lgrambox_γ:
    movq  %r14, (%rbx)
    addq  $8, %rsp
    popq  %rcx
    popq  %rbx
    popq  %r15
    popq  %r14
    popq  %r13
    ret
.Lgrambox_ω:
    movl  $DT_FAIL, %eax
    xorl  %edx, %edx
    movq  %r14, (%rbx)
    addq  $8, %rsp
    popq  %rcx
    popq  %rbx
    popq  %r15
    popq  %r14
    popq  %r13
    ret
    .size  rk_gram_enter_box, .-rk_gram_enter_box
    .section .note.GNU-stack,"",@progbits
