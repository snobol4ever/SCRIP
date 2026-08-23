/* rt_sg_scan.S — SG-2 SPAN-GUTS level-3 leafs (GOAL-SNOBOL4-BB.md SG ladder, Lon directive s126).                                                                                                    */
/* LEAN CUSTOM CONVENTION — NOT the C ABI (no xfer dance, no alignment requirement: leafs make no calls, touch no stack beyond ret, touch no xmm).  R13/R15-AWARE: subject read directly from the      */
/* match-register contract R13=Sigma subject base, R15d=Delta subject length (R14/delta NOT read — caller passes the start).  NUL-SAFE BY EXPLICIT LENGTHS: subject bounded by R15d, needle bounded    */
/* by edx; byte value 0 is an ordinary byte on both sides (Lon s126: strchr truncates a NUL-bearing needle AND strchr(s,0) matches the terminator, falsely accepting a NUL subject byte as a member).  */
/* ⭐⭐ MEMBERSHIP IS A 256-BIT MAP, NOT A RESCAN OF THE NEEDLE (hq_P s266).  The scan arms used to walk the WHOLE needle for EVERY subject byte -- O(subject x needle) -- which     */
/* is 6 + 5*len instructions per byte and made rt_sg_scan_member 7.7% of the json deserializer on a 631 KB input (BREAK over a 4-character cset touches essentially every byte).   */
/* Now the needle is burned once into a 32-byte bit map in the leaf's own frame and each subject byte costs a load, a shift and one bt: 10 instructions, independent of the needle. */
/* ⛔ THE SHORT ARMS ARE KEPT AND ARE STILL THE DEFAULT BELOW THE CROSSOVER.  Building the map costs ~4 + 5*len before the first subject byte is read, so for len < 3 -- or with     */
/* fewer than 2 bytes left to scan -- the original linear walk is genuinely cheaper and is what runs.  A cset is not a datatype here: this is a size crossover, not an op filter.    */
/* ⛔ THE LEAF NOW TOUCHES ITS OWN STACK, which the note below said it never did.  32 bytes, sub/add around the whole map arm, no call in between, return address untouched.        */
/* NUL-safety is unchanged: the map is indexed by the byte VALUE, so 0 is an ordinary member and nothing is length-terminated.  rsi/rdx still survive; rax rcx rdi r8 r9 clobbered.  */
/* rt_sg_scan_member:    in edi=start pos, rsi=needle ptr, edx=needle len; out eax=first pos >= start with subject[pos] IN needle, or Delta (r15d) if none.  Clobbers rax rcx rdi r8 r9d ONLY.         */
/* rt_sg_scan_nonmember: same contract; out eax=first pos >= start with subject[pos] NOT IN needle, or Delta if none.                                                                                 */
/* rt_sg_member:         in edi=char, rsi=needle ptr, edx=needle len; out eax=1 if char IN needle else 0.  Clobbers rax rcx r9d ONLY.                                                                 */
    .text
    .globl rt_sg_scan_member
    .type  rt_sg_scan_member, @function
rt_sg_scan_member:
    movl %edi, %eax
    cmpl $3, %edx
    jl   1f                          /* short needle: the linear form is already cheapest */
    movl %r15d, %ecx
    subl %eax, %ecx
    cmpl $2, %ecx
    jl   1f                          /* nothing left to amortise a map over               */
    subq $32, %rsp                   /* the 256-bit membership map, in the leaf's own frame */
    xorl %r8d, %r8d
    movq %r8, (%rsp)
    movq %r8, 8(%rsp)
    movq %r8, 16(%rsp)
    movq %r8, 24(%rsp)
    xorl %ecx, %ecx
5:  cmpl %edx, %ecx
    jge  6f
    movzbl (%rsi,%rcx,1), %r8d
    btsq %r8, (%rsp)                 /* bit-string form: byte = rsp + 8*(c>>6), bit = c&63  */
    addl $1, %ecx
    jmp  5b
6:  movslq %eax, %rcx
    addq %r13, %rcx                  /* p                                                   */
    movslq %r15d, %rdi
    addq %r13, %rdi                  /* end                                                 */
7:  cmpq %rdi, %rcx
    jae  9f
    movzbl (%rcx), %r8d
    movl %r8d, %r9d
    shrl $6, %r9d
    movq (%rsp,%r9,8), %r9
    btq  %r8, %r9                    /* register form: fast, and btq masks the offset to 63 */
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
