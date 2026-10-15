    .text
    .globl rt_sg_scan_member
    .type  rt_sg_scan_member, @function
rt_sg_scan_member:
    movl %edi, %eax
    leal -1(%rdx), %ecx
    cmpl $1, %ecx
    jbe  .Lsgm_fast
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
.Lsgm_fast:
    movl %r15d, %ecx
    subl %eax, %ecx
    jle  .Lsgm_ret
    movslq %eax, %r8
    addq %r13, %r8
    movq %r8, %xmm2
    movslq %r15d, %r9
    addq %r13, %r9
    movabsq $0x0101010101010101, %rdi
    movzbl (%rsi), %r10d
    imulq %rdi, %r10
    cmpl $2, %edx
    je   .Lsgm_two
    movabsq $0x8080808080808080, %rsi
    leaq -8(%r9), %rcx
    cmpq %rcx, %r8
    ja   .Lsgm_otail
.Lsgm_o8:
    movq (%r8), %rdx
    xorq %r10, %rdx
    movq %rdx, %rax
    notq %rax
    subq %rdi, %rdx
    andq %rax, %rdx
    andq %rsi, %rdx
    jnz  .Lsgm_hit
    addq $8, %r8
    cmpq %rcx, %r8
    jbe  .Lsgm_o8
.Lsgm_otail:
    cmpq %r9, %r8
    jae  .Lsgm_none
    movq %xmm2, %rax
    cmpq %rax, %rcx
    jb   .Lsgm_obyte
    movq %rcx, %r8
    jmp  .Lsgm_o8
.Lsgm_obyte:
    cmpb %r10b, (%r8)
    je   .Lsgm_foundb
    addq $1, %r8
    cmpq %r9, %r8
    jb   .Lsgm_obyte
    jmp  .Lsgm_none
.Lsgm_two:
    movzbl 1(%rsi), %r11d
    imulq %rdi, %r11
    movabsq $0x8080808080808080, %rsi
    leaq -8(%r9), %rax
    cmpq %rax, %r8
    ja   .Lsgm_ttail
.Lsgm_t8:
    movq (%r8), %rdx
    movq %rdx, %rcx
    xorq %r10, %rdx
    xorq %r11, %rcx
    movq %rdx, %rax
    notq %rax
    subq %rdi, %rdx
    andq %rax, %rdx
    movq %rcx, %rax
    notq %rax
    subq %rdi, %rcx
    andq %rax, %rcx
    orq  %rcx, %rdx
    andq %rsi, %rdx
    jnz  .Lsgm_hit
    addq $8, %r8
    leaq -8(%r9), %rax
    cmpq %rax, %r8
    jbe  .Lsgm_t8
.Lsgm_ttail:
    cmpq %r9, %r8
    jae  .Lsgm_none
    movq %xmm2, %rax
    leaq -8(%r9), %rcx
    cmpq %rax, %rcx
    jb   .Lsgm_tbyte
    movq %rcx, %r8
    jmp  .Lsgm_t8
.Lsgm_tbyte:
    cmpb %r10b, (%r8)
    je   .Lsgm_foundb
    cmpb %r11b, (%r8)
    je   .Lsgm_foundb
    addq $1, %r8
    cmpq %r9, %r8
    jb   .Lsgm_tbyte
.Lsgm_none:
    movl %r15d, %eax
    ret
.Lsgm_hit:
    bsfq %rdx, %rdx
    shrl $3, %edx
    subq %r13, %r8
    leal (%r8,%rdx), %eax
    ret
.Lsgm_foundb:
    subq %r13, %r8
    movl %r8d, %eax
.Lsgm_ret:
    ret
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
