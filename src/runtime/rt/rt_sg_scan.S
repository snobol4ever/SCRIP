    .text
    .globl rt_sg_scan_member
    .type  rt_sg_scan_member, @function
rt_sg_scan_member:
    movl %edi, %eax
    cmpl $3, %edx
    jl   1f
    movl %r15d, %ecx
    subl %eax, %ecx
    cmpl $2, %ecx
    jl   1f
    subq $32, %rsp
    xorl %r8d, %r8d
    movq %r8, (%rsp)
    movq %r8, 8(%rsp)
    movq %r8, 16(%rsp)
    movq %r8, 24(%rsp)
    xorl %ecx, %ecx
5:  cmpl %edx, %ecx
    jge  6f
    movzbl (%rsi,%rcx,1), %r8d
    btsq %r8, (%rsp)
    addl $1, %ecx
    jmp  5b
6:  movslq %eax, %rcx
    addq %r13, %rcx
    movslq %r15d, %rdi
    addq %r13, %rdi
7:  cmpq %rdi, %rcx
    jae  9f
    movzbl (%rcx), %r8d
    movl %r8d, %r9d
    shrl $6, %r9d
    movq (%rsp,%r9,8), %r9
    btq  %r8, %r9
    jc   8f
    addq $1, %rcx
    jmp  7b
8:  subq %r13, %rcx
    movl %ecx, %eax
    addq $32, %rsp
    ret
9:  movl %r15d, %eax
    addq $32, %rsp
    ret
1:  cmpl %r15d, %eax
    jge  4f
    movslq %eax, %rcx
    movzbl (%r13,%rcx,1), %edi
    xorl %ecx, %ecx
2:  cmpl %edx, %ecx
    jge  3f
    movzbl (%rsi,%rcx,1), %r9d
    cmpl %r9d, %edi
    je   4f
    addl $1, %ecx
    jmp  2b
3:  addl $1, %eax
    jmp  1b
4:  ret
    .size rt_sg_scan_member, .-rt_sg_scan_member
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_sg_scan_nonmember
    .type  rt_sg_scan_nonmember, @function
rt_sg_scan_nonmember:
    movl %edi, %eax
    cmpl $3, %edx
    jl   1f
    movl %r15d, %ecx
    subl %eax, %ecx
    cmpl $2, %ecx
    jl   1f
    subq $32, %rsp
    xorl %r8d, %r8d
    movq %r8, (%rsp)
    movq %r8, 8(%rsp)
    movq %r8, 16(%rsp)
    movq %r8, 24(%rsp)
    xorl %ecx, %ecx
5:  cmpl %edx, %ecx
    jge  6f
    movzbl (%rsi,%rcx,1), %r8d
    btsq %r8, (%rsp)
    addl $1, %ecx
    jmp  5b
6:  movslq %eax, %rcx
    addq %r13, %rcx
    movslq %r15d, %rdi
    addq %r13, %rdi
7:  cmpq %rdi, %rcx
    jae  9f
    movzbl (%rcx), %r8d
    movl %r8d, %r9d
    shrl $6, %r9d
    movq (%rsp,%r9,8), %r9
    btq  %r8, %r9
    jnc  8f
    addq $1, %rcx
    jmp  7b
8:  subq %r13, %rcx
    movl %ecx, %eax
    addq $32, %rsp
    ret
9:  movl %r15d, %eax
    addq $32, %rsp
    ret
1:  cmpl %r15d, %eax
    jge  4f
    movslq %eax, %rcx
    movzbl (%r13,%rcx,1), %edi
    xorl %ecx, %ecx
2:  cmpl %edx, %ecx
    jge  4f
    movzbl (%rsi,%rcx,1), %r9d
    cmpl %r9d, %edi
    je   3f
    addl $1, %ecx
    jmp  2b
3:  addl $1, %eax
    jmp  1b
4:  ret
    .size rt_sg_scan_nonmember, .-rt_sg_scan_nonmember
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    .globl rt_sg_member
    .type  rt_sg_member, @function
rt_sg_member:
    xorl %eax, %eax
    xorl %ecx, %ecx
1:  cmpl %edx, %ecx
    jge  2f
    movzbl (%rsi,%rcx,1), %r9d
    cmpl %r9d, %edi
    je   3f
    addl $1, %ecx
    jmp  1b
3:  movl $1, %eax
2:  ret
    .size rt_sg_member, .-rt_sg_member
    .section .note.GNU-stack,"",@progbits
