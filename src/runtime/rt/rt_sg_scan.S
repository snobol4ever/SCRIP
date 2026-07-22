/* rt_sg_scan.S — SG-2 SPAN-GUTS level-3 leafs (GOAL-SNOBOL4-BB.md SG ladder, Lon directive s126).                                                                                                    */
/* LEAN CUSTOM CONVENTION — NOT the C ABI (no xfer dance, no alignment requirement: leafs make no calls, touch no stack beyond ret, touch no xmm).  R13/R15-AWARE: subject read directly from the      */
/* match-register contract R13=Sigma subject base, R15d=Delta subject length (R14/delta NOT read — caller passes the start).  NUL-SAFE BY EXPLICIT LENGTHS: subject bounded by R15d, needle bounded    */
/* by edx; byte value 0 is an ordinary byte on both sides (Lon s126: strchr truncates a NUL-bearing needle AND strchr(s,0) matches the terminator, falsely accepting a NUL subject byte as a member).  */
/* rt_sg_scan_member:    in edi=start pos, rsi=needle ptr, edx=needle len; out eax=first pos >= start with subject[pos] IN needle, or Delta (r15d) if none.  Clobbers rax rcx rdi r8 r9d ONLY.         */
/* rt_sg_scan_nonmember: same contract; out eax=first pos >= start with subject[pos] NOT IN needle, or Delta if none.                                                                                 */
/* rt_sg_member:         in edi=char, rsi=needle ptr, edx=needle len; out eax=1 if char IN needle else 0.  Clobbers rax rcx r9d ONLY.                                                                 */
    .text
    .globl rt_sg_scan_member
    .type  rt_sg_scan_member, @function
rt_sg_scan_member:
    movl %edi, %eax
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
