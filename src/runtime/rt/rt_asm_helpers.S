/* rt_asm_helpers.S — hand-written x86-64 fast paths for the SMALL, HOTTEST runtime leaves (Lon directive: asm for small helpers, C for large; -O0/-O1 only, no -O2 in this container).            */
/* SysV AMD64 ABI, matched byte-for-byte to the SCRIP emitter's own DESCR_t marshaling (verified vs emitted caller in a mode-4 .s: arg DESCR_t in RDI:RSI = {v(lo32),slen(hi32)} : payload, return   */
/* DESCR_t in RAX:RDX).  descr.h layout: v @0 (4B), slen @4 (4B), payload @8 (8B); DT_N=9, DT_FAIL=99; NAMETRAP = {v=9,slen=2,p=VCELL*}; VCELL_t.cellp @0.  The three HOT deref shapes are inlined here  */
/* (slen==1 pointer cell, slen==0 shared-NV name, slen==2 nametrap with a direct cellp); every COLD shape (nametrap table lookup, varref substring, malformed) tail-jumps to the full C rt_deref_slow.  */
    .text
    .globl rt_deref
    .type  rt_deref, @function
rt_deref:
    movl   %edi, %eax
    cmpl   $9, %eax
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
    movl   $99, %eax
    xorl   %edx, %edx
    ret
.Lrd_slow:
    jmp    rt_deref_slow@PLT
    .size  rt_deref, .-rt_deref
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* int64_t to_int(DESCR_t v): arg RDI:RSI = {v,slen}:payload, return int64_t in RAX.  Hot case DT_I(6) returns v.i (RSI) with two instructions; DT_R/DT_S/DT_K/error delegate to the full C to_int_slow. */
    .globl to_int
    .type  to_int, @function
to_int:
    cmpl   $6, %edi
    jne    .Lti_slow
    movq   %rsi, %rax
    ret
.Lti_slow:
    jmp    to_int_slow@PLT
    .size  to_int, .-to_int
    .section .note.GNU-stack,"",@progbits
